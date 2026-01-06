#include "TrayIcon.h"

namespace MIDIRouterUtils
{
static void drawLogoImage(juce::Graphics& g, int size)
{
    auto font = juce::Font(juce::FontOptions()).withHeight(95);

    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(juce::FontOptions()).withHeight(95));
    g.drawFittedText("MIDI", {0, 0, size, size}, juce::Justification::centred, 1);
}

juce::Image getLogoImage(int size)
{
    auto newImage = juce::Image(juce::Image::ARGB, size, size, true);

    {
        auto g = juce::Graphics(newImage);
        drawLogoImage(g, size);
    }

    return newImage;
}
} // namespace MIDIRouterUtils