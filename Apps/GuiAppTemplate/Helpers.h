#pragma once
#include "State.h"

namespace MIDIRouterApp::Helpers
{

    inline juce::PopupMenu getOutputsMenu(State& state,
                                          const juce::MidiDeviceInfo& inputInfo)
    {
        auto menu    = juce::PopupMenu();
        auto outputs = juce::MidiOutput::getAvailableDevices();

        for (auto& output : outputs)
        {
            const bool ticked = state.isRouted(inputInfo, output);
            menu.addItem(output.name, true, ticked,
                         [&state, inputInfo, output] { state.toggleConnection(inputInfo, output); });
        }

        if (outputs.isEmpty())
            menu.addItem("(no MIDI outputs found)", false, false, nullptr);

        return menu;
    }

    inline juce::PopupMenu getMenu(State& state)
    {
        auto menu   = juce::PopupMenu();
        auto inputs = juce::MidiInput::getAvailableDevices();

        for (auto& input : inputs)
            menu.addSubMenu(input.name, getOutputsMenu(state, input), true);

        if (inputs.isEmpty())
            menu.addItem("(no MIDI inputs found)", false, false, nullptr);

        menu.addSeparator();
        menu.addItem("Quit", [] { juce::JUCEApplication::getInstance()->quit(); });

        return menu;
    }

}  // namespace MIDIRouterApp::Helpers
