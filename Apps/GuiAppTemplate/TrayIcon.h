#pragma once

#include "State.h"
#include <BinaryData.h>

namespace MIDIRouterApp
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

} // namespace GuiApp