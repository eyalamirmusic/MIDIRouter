#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

struct MenuLookAndFeel : public juce::LookAndFeel_V4
{
    int getDefaultMenuBarHeight() override {return 24;}
    juce::Font getMenuBarFont(juce::MenuBarComponent&, int itemIndex, const juce::String& itemText) override
    {
        juce::ignoreUnused(itemIndex, itemText);
        return juce::Font(juce::FontOptions().withHeight(14.0f));
    }

};