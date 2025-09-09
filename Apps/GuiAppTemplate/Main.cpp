#include "TrayIcon.h"
#include "Router.h"

namespace MIDIRouterApp
{
class SineSound : public juce::SynthesiserSound {
public: bool appliesToNote (int) override { return true; }
        bool appliesToChannel (int) override { return true; } };

class SineVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound (juce::SynthesiserSound* s) override { return dynamic_cast<SineSound*>(s) != nullptr; }
    void startNote (int note, float vel, juce::SynthesiserSound*, int) override
    {
        DBG("startNote note=" << note << "vel=" << vel);

        //BUGFIX: GUI was sending MIDI fine, but tailoff never got reset
        //After the first note tailed to near zero, new notes were multiplied
        //by the tiny tailoff value (creating near silence)
        //FIX: reset tailoff here so every note starts clean
        tailOff = 0.0f; //reset tail
        level = juce::jlimit (0.05f, 1.0f, vel);
        currentAngle = 0.0;
        const auto hz = juce::MidiMessage::getMidiNoteInHertz (note);
        angleDelta = (hz / getSampleRate()) * juce::MathConstants<double>::twoPi;
    }
    void stopNote (float, bool allowTailOff) override
    {
        DBG("stopNote (tail=" << (allowTailOff ? "on" : "off") << ")" );
        if (allowTailOff)
        {
            if (angleDelta != 0.0)
                tailOff = 1.0f; // tailoff AFTER sounding
        }
        else
        {
            clearCurrentNote();
            angleDelta = 0.0;
            tailOff    = 0.0f; //clean state
        }
    }
    void pitchWheelMoved (int) override {}
    void controllerMoved (int, int) override {}
    void renderNextBlock (juce::AudioBuffer<float>& buf, int start, int num) override
    {
        if (angleDelta == 0.0) return; //idle voice, do nothing
        auto* L = buf.getWritePointer (0, start);
        auto* R = buf.getNumChannels() > 1 ? buf.getWritePointer (1, start) : nullptr;
        while (num--)
        {
            float s = (float) (std::sin(currentAngle) * level);
            currentAngle += angleDelta;

            if (tailOff > 0.0)
            {
                s *= (float) tailOff;
                tailOff *= 0.99;

                if (tailOff <= 0.005f)
                {
                    clearCurrentNote();
                    angleDelta=0.0;
                    // break;
                    s = 0.0f; //write zeros for rest of block
                }
            }
            *L++ += s; //adding/mixing instead of overwriting
            if (R) *R++ += s;
        }
    }
private:
    double currentAngle = 0.0, angleDelta = 0.0, tailOff = 0.0;
    float  level = 0.0f;
};

class TestSynthAudio : public juce::AudioSource
{
public:
    TestSynthAudio()
    {
        for (int i = 0; i < 8; ++i) synth.addVoice (new SineVoice());
        synth.addSound (new SineSound());
        // collector.reset (44100.0);
    }



    juce::MidiKeyboardState    keyboardState;   // for on-screen keyboard
    juce::MidiMessageCollector collector;       // router will push MIDI here

    void prepareToPlay (int, double sr) override
    {
        if ( sr <= 0.0 ) sr = 44100.0;
        synth.setCurrentPlaybackSampleRate(sr);
        collector.reset(sr);
        keyboardState.reset();
    }
    void releaseResources() override {}
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& info) override
    {
        info.clearActiveBufferRegion();

        juce::MidiBuffer midi;
        collector.removeNextBlockOfMessages (midi, info.numSamples);
        keyboardState.processNextMidiBuffer (midi, 0, info.numSamples, true);
        synth.renderNextBlock (*info.buffer, midi, 0, info.numSamples);
    }
private:
    juce::Synthesiser synth;
};

class TestSynthWindow : public juce::DocumentWindow
{
public:
    TestSynthWindow (juce::MidiKeyboardState& state)
    : juce::DocumentWindow ("Test Synth",
        juce::Desktop::getInstance().getDefaultLookAndFeel()
            .findColour (juce::ResizableWindow::backgroundColourId),
        DocumentWindow::allButtons),
      keyboard (state, juce::MidiKeyboardComponent::horizontalKeyboard)
    {
        setUsingNativeTitleBar (true);
        setResizable (true, true);

        // 👇 add this: use constant full velocity for mouse clicks
        keyboard.setVelocity (1.0f, /*mousePositionBased*/ false);

        auto* c = new juce::Component();
        c->addAndMakeVisible (keyboard);
        c->setSize (640, 200);
        setContentOwned (c, true);
        centreWithSize (660, 240);
        setVisible (true);
    }
    void closeButtonPressed() override { setVisible(false); }
    void resized() override { if (auto* c = getContentComponent()) keyboard.setBounds (c->getLocalBounds().reduced(8)); }
private:
    juce::MidiKeyboardComponent keyboard;
};

struct App
{
    State state;
    MIDIRouter router {state};
    DummyMenuBarModel dummyMenuBar;
    TrayIcon icon {state};

    juce::AudioDeviceManager dm;
    TestSynthAudio testAudio;
    std::unique_ptr<juce::AudioSourcePlayer> player;
    std::unique_ptr<TestSynthWindow> testWindow;
};

class GuiAppTemplateApplication : public juce::JUCEApplication
{
public:
    GuiAppTemplateApplication() { juce::Process::setDockIconVisible(false); }

private:
    const String getApplicationName() override { return JUCE_APPLICATION_NAME_STRING; }
    const String getApplicationVersion() override
    {
        return JUCE_APPLICATION_VERSION_STRING;
    }
    bool moreThanOneInstanceAllowed() override { return false; }

    void initialise(const String&) override
    {
        app = std::make_unique<App>();

        app->dm.initialiseWithDefaultDevices(0,2);
        app->player= std::make_unique<juce::AudioSourcePlayer>();
        app->player->setSource((&app->testAudio));
        app->dm.addAudioCallback((app->player.get()));

        app->router.midiTap = &app->testAudio.collector;

        app->testWindow = std::make_unique<TestSynthWindow> (app->testAudio.keyboardState);

    }

    void shutdown() override
    {
        if (app)
        {
            if (app->player)
            {
                app->dm.removeAudioCallback( (app->player.get()));
                app->player->setSource(nullptr);
            }
        app.reset();
        }
    }

    void systemRequestedQuit() override { quit(); }

    void anotherInstanceStarted(const String& /*commandLine*/) override {}

    std::unique_ptr<App> app;
};

} // namespace MIDIRouterApp

// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(MIDIRouterApp::GuiAppTemplateApplication)
