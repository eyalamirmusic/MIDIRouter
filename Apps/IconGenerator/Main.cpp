#include <ea_midirouter_ui_utils/ea_midirouter_ui_utils.h>

using juce::File;

File createTargetFile()
{
    auto file = File::getSpecialLocation(File::userDesktopDirectory)
                    .getChildFile("MIDIRouterIcon.png");

    if (file.existsAsFile())
        file.deleteFile();

    file.create();

    return file;
}

int main()
{
    auto init = juce::ScopedJuceInitialiser_GUI();

    auto image = MIDIRouterUtils::getIconImage();
    auto file = createTargetFile();

    if (auto stream = file.createOutputStream())
        juce::PNGImageFormat().writeImageToStream(image, *stream);

    return 0;
}