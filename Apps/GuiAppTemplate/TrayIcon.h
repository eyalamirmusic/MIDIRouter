#pragma once

#include "State.h"
#include "Helpers.h"

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

    void mouseDown(const juce::MouseEvent&) override
    {
        showDropdownMenu(Helpers::getMenu(state));
    }

    State& state;
};

} // namespace MIDIRouterApp