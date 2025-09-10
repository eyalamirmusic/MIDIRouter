#pragma once

#include "State.h"

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
    auto newImage = juce::Image(juce::Image::ARGB, 128, 128, true);

    {
        auto g = juce::Graphics(newImage);
        g.setColour(juce::Colours::white);
        g.setFont(juce::Font(juce::FontOptions()).withHeight(95));
        g.drawFittedText("MIDI", {0,0, 128, 128}, juce::Justification::centred, 1);
    }

    return newImage;
}

struct TrayIcon : juce::SystemTrayIconComponent
{
    TrayIcon(State& stateToUse)
        : state(stateToUse)
    {
        setIconImage(getLogoImage(), getLogoImage());
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

    // juce::PopupMenu getMenu() const
    // {
    //     auto menu = juce::PopupMenu();
    //     menu.addSubMenu("Add Connection", createAddConnectionSubMenu());
    //
    //     for (auto& connection: state.connections)
    //     {
    //         menu.addSubMenu(connection.input.name, getConnectionMenu(connection));
    //     }
    //
    //     menu.addItem("Quit", [] { juce::JUCEApplication::getInstance()->quit(); });
    //
    //     return menu;
    // }

    juce::PopupMenu getMenu() //dropped const
    {
        auto menu = juce::PopupMenu();

        auto inputs = juce::MidiInput::getAvailableDevices();

        for (auto &input: inputs) {
            if (state.hasConnection(input)) {
                for (auto &connection: state.connections) {
                    if (connection.input == input) {
                        menu.addSubMenu(input.name,
                                        getConnectionMenu(connection),
                                        true);
                    }
                }
            } else {
                auto addItemFunc = [input, this] { state.createConnection(input); };
                menu.addItem(input.name, addItemFunc);
            }
        }

        if (inputs.isEmpty())
            menu.addItem("no inputs found", false, false, [] {
            });

        menu.addSeparator();
        menu.addItem("Quit", [] { juce::JUCEApplication::getInstance()->quit(); });

        return menu;
    }

    void mouseDown(const juce::MouseEvent&) override { showDropdownMenu(getMenu()); }

    State& state;
};

} // namespace MIDIRouterApp