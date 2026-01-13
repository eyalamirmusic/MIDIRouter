#pragma once

#include "State.h"
#include <ea_midirouter_ui_utils/ea_midirouter_ui_utils.h>

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

struct TrayIcon : juce::SystemTrayIconComponent
{
    TrayIcon(State& stateToUse)
        : state(stateToUse)
    {
        auto image = MIDIRouterUtils::getLogoImage(128);
        setIconImage(image, image);
        setIconTooltip("I'm the MIDI router! Good stuff");
    }

    void resized() override
    {
        DBG("Resized");
        DBG(getLocalBounds().toString());
    }

    juce::PopupMenu createAddConnectionSubMenu() const
    {
        auto menu = juce::PopupMenu();

        auto inputs = RTMIDI::getPortList().inputs;

        for (auto& input: inputs)
        {
            if (!state.hasConnection(input.name))
            {
                auto addItemFunc = [input, this] { state.createConnection(input.name); };
                menu.addItem(input.name, addItemFunc);
            }
        }

        return menu;
    }

    static juce::PopupMenu getConnectionMenu(ConnectionDescription& connection)
    {
        auto menu = juce::PopupMenu();

        menu.addItem("In:" + connection.input, false, false, [] {});

        auto outputs = RTMIDI::getPortList().outputs;

        for (auto& output: outputs)
        {
            auto connectFunc = [output, &connection] { connection.toggle(output.name); };
            menu.addItem(
                output.name, true, connection.hasOutput(output.name), connectFunc);
        }

        return menu;
    }

    juce::PopupMenu getMenu() const
    {
        auto menu = juce::PopupMenu();
        menu.addSubMenu("Add Connection", createAddConnectionSubMenu());

        for (auto& connection: state.connections)
        {
            menu.addSubMenu(connection.input, getConnectionMenu(connection));
        }

        menu.addItem("Restart Connections", [&] { state.rebuildConnections(); });
        menu.addItem("Quit", [] { juce::JUCEApplication::getInstance()->quit(); });

        return menu;
    }

    void mouseDown(const juce::MouseEvent&) override { showDropdownMenu(getMenu()); }

    State& state;
};

} // namespace MIDIRouterApp