#pragma once

#include "../Source/CommonHeader.h"
#include <juce_audio_utils/juce_audio_utils.h>

namespace GuiApp
{
class MainWindow : public juce::DocumentWindow
{
public:
    MainWindow(const String& name);

private:
    void closeButtonPressed() override;
    Colour getBackgroundColour();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};
}

