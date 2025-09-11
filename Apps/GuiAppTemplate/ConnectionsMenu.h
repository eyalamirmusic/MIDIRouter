#pragma once

#include "State.h"
#include "Helpers.h"

namespace MIDIRouterApp
{
struct ConnectionsMenu : public juce::MenuBarModel
{
    ConnectionsMenu(State &stateToUse) : state(stateToUse) {}

    juce::StringArray getMenuBarNames() override { return {"MIDI Connections"}; }


    void menuItemSelected(int, int) override {}

    juce::PopupMenu getMenuForIndex(int topLevelMenuIndex, const juce::String&) override
    {
        if (topLevelMenuIndex != 0)
            return {};

        return Helpers::getMenu(state);
    }

    State &state;
};
}
