#include "../Source/MainComponent.h"
#include <BinaryData.h>

namespace GuiApp
{
class DummyMenuBarModel final : public juce::MenuBarModel
{
public:
    DummyMenuBarModel() { setMacMainMenu(this); }
    ~DummyMenuBarModel() override { setMacMainMenu(nullptr); }

    juce::StringArray getMenuBarNames() override { return {""}; }
    juce::PopupMenu getMenuForIndex(int, const String&) override { return {}; }
    void menuItemSelected(int, int) override {}
};

inline juce::Image getLogoImage()
{
    return juce::ImageCache::getFromMemory(BinaryData::Logo_png,
                                           BinaryData::Logo_pngSize);
}

struct TrayIcon : juce::SystemTrayIconComponent
{
    TrayIcon(State& stateToUse)
        : state(stateToUse)
    {
        setIconImage(getLogoImage(), getLogoImage());
        setIconTooltip("I'm the MIDI router! Good stuff");
    }

    juce::PopupMenu createAddConnectionSubMenu() const
    {
        auto menu = juce::PopupMenu();

        auto inputs = juce::MidiInput::getAvailableDevices();

        for (auto& input: inputs)
        {
            if (!state.hasConnection(input))
            {
                auto addItemFunc = [input, this] { state.createConnection(input); };
                menu.addItem(input.name, addItemFunc);
            }
        }

        return menu;
    }

    static juce::PopupMenu getConnectionMenu(ConnectionDescription& connection)
    {
        auto menu = juce::PopupMenu();

        menu.addItem("In:" + connection.input.name, false, false, [] {});

        auto outputs = juce::MidiOutput::getAvailableDevices();

        for (auto& output: outputs)
        {
            auto connectFunc = [output, &connection] { connection.toggle(output); };
            menu.addItem(output.name, true, connection.hasOutput(output), connectFunc);
        }

        return menu;
    }

    juce::PopupMenu getMenu() const
    {
        auto menu = juce::PopupMenu();
        menu.addSubMenu("Add Connection", createAddConnectionSubMenu());

        for (auto& connection: state.connections)
        {
            menu.addSubMenu(connection.input.name, getConnectionMenu(connection));
        }

        menu.addItem("Quit", [] { juce::JUCEApplication::getInstance()->quit(); });

        return menu;
    }

    void mouseDown(const juce::MouseEvent&) override { showDropdownMenu(getMenu()); }

    State& state;
};

struct App
{
    State state;
    MIDIRouter router {state};
    DummyMenuBarModel dummyMenuBar;
    TrayIcon icon {state};
};

class GuiAppTemplateApplication : public juce::JUCEApplication
{
public:
    const String getApplicationName() override { return JUCE_APPLICATION_NAME_STRING; }
    const String getApplicationVersion() override
    {
        return JUCE_APPLICATION_VERSION_STRING;
    }
    bool moreThanOneInstanceAllowed() override { return true; }

    void initialise(const String&) override { app = std::make_unique<App>(); }

    void shutdown() override { app.reset(); }

    void systemRequestedQuit() override { quit(); }

    void anotherInstanceStarted(const String& /*commandLine*/) override {}

private:
    std::unique_ptr<App> app;
};

} // namespace GuiApp

// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(GuiApp::GuiAppTemplateApplication)
