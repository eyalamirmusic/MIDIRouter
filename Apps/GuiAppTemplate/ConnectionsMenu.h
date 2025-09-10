#pragma once

#include "State.h"
#include "TrayIcon.h"

namespace MIDIRouterApp
{
struct ConnectionsMenu : public juce::MenuBarModel
{
    ConnectionsMenu(State &stateToUse) : state(stateToUse) {}

    juce::StringArray getMenuBarNames() override { return {"MIDI Connections"}; }

    juce::PopupMenu getMenuForIndex(int topLevelMenuIndex, const juce::String&) override
    {
        auto menu = juce::PopupMenu();
        if (topLevelMenuIndex != 0)
            return menu;
        auto inputs = juce::MidiInput::getAvailableDevices();
        for (auto &input: inputs)
        {
            if (state.hasConnection(input))
            {
                for (auto &connection: state.connections)
                {
                    if (connection.input == input)
                    {
                        menu.addSubMenu(input.name,
                                        TrayIcon::getConnectionMenu(connection),
                                        true);
                        break;
                    }
                }

            }
            else
            {
                auto addItemFunc = [input, this] { state.createConnection(input); };
                menu.addItem(input.name, addItemFunc);
            }
        }

        if (inputs.isEmpty())
            menu.addItem("no inputs found", false, false, [] {});

        menu.addSeparator();
        menu.addItem("Quit", [] { juce::JUCEApplication::getInstance()->quit(); });

        return menu;
    }

    void menuItemSelected(int, int) override {}

    State &state;
};
}
