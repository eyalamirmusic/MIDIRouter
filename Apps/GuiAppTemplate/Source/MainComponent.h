#pragma once

#include "HelloWorldLabel.h"
#include <juce_audio_utils/juce_audio_utils.h>

namespace GuiApp
{
struct RoutingCallback : juce::MidiInputCallback
{
    RoutingCallback(juce::MidiOutput& outputToUse)
        : output(outputToUse)
    {
        DBG("Creating router");
    }

    void handleIncomingMidiMessage(juce::MidiInput*,
                                   const juce::MidiMessage& message) override
    {
        DBG(message.getDescription());
        output.sendMessageNow(message);
    }

    juce::MidiOutput& output;
};

struct MIDIConnector
{
    MIDIConnector()
    {
        auto outputs = juce::MidiOutput::getAvailableDevices();

        for (auto& output: outputs)
        {
            if (output.name.equalsIgnoreCase("USB MIDI Device Port 1"))
            {
                midiOut = juce::MidiOutput::openDevice(output.identifier);
            }
        }

        auto inputs = juce::MidiInput::getAvailableDevices();

        for (auto& input: inputs)
        {
            if (input.name.contains("M-Audio Hammer 88"))
            {
                cb = std::make_unique<RoutingCallback>(*midiOut);
                midiIn = juce::MidiInput::openDevice(input.identifier, cb.get());
                midiIn->start();
            }
        }
    }

    std::unique_ptr<RoutingCallback> cb;
    std::unique_ptr<juce::MidiInput> midiIn;
    std::unique_ptr<juce::MidiOutput> midiOut;
};

class MainComponent : public Component
{
public:
    MainComponent();

    void paint(Graphics&) override;
    void resized() override;

private:
    MIDIConnector connector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
} // namespace GuiApp