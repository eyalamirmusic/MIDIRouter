#include "TrayIcon.h"

namespace MIDIRouterUtils
{

static float getFontHeight(int size)
{
    return (float) size * 0.74f;
}

static void drawLogoImage(juce::Graphics& g, int size, const juce::Colour& bg)
{
    auto height = getFontHeight(size);
    auto font = juce::Font(juce::FontOptions()).withHeight(height);

    g.setColour(bg);
    g.setFont(font);
    g.drawFittedText("MIDI", {0, 0, size, size}, juce::Justification::centred, 1);
}

juce::Image getLogoImage(int size, const juce::Colour& bgColor)
{
    auto newImage = juce::Image(juce::Image::ARGB, size, size, true);

    {
        auto g = juce::Graphics(newImage);
        drawLogoImage(g, size, bgColor);
    }

    return newImage;
}

juce::Image getIconImage()
{
    return getLogoImage(1024, juce::Colours::black);
}
} // namespace MIDIRouterUtils