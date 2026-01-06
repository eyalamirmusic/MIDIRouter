#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace MIDIRouterUtils
{
juce::Image getLogoImage(int size,
                         const juce::Colour& bgColor = juce::Colours::white);

juce::Image getIconImage();
} // namespace MIDIRouterUtils