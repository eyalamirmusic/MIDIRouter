#pragma once
#include "ConnectionsMenu.h"
#include "MenuLookAndFeel.h"

namespace MIDIRouterApp
{
struct ConnectionsMenuBar : juce::Component
{
    ConnectionsMenuBar(State &stateToUse) : menu(stateToUse), bar(&menu)
    {
        bar.setLookAndFeel(&laf);
        addAndMakeVisible(bar);
    }

    void resized() override
    {
        const int barHeight = 24;
        const int barWidth  = 200;
        auto centered = getLocalBounds().withSizeKeepingCentre(barWidth, barHeight);

        bar.setBounds(centered);
    }

    MenuLookAndFeel laf;
    ConnectionsMenu menu;
    juce::MenuBarComponent bar;
};

struct ConnectionsWindow : juce::DocumentWindow
{
    ConnectionsWindow(State &stateToUse)
        : DocumentWindow("MIDI Router",
                         juce::Colours::black,
                         DocumentWindow::allButtons)
    {
        setUsingNativeTitleBar(true);
        setResizable(false, false);
        setContentOwned(new ConnectionsMenuBar(stateToUse), true);
        centreWithSize(420, 60);
        setVisible(true);
    }

    void closeButtonPressed() override
    {
        setVisible(false);
    }
};
}
