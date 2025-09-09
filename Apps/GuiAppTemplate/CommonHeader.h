#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <nlohmann/json.hpp>

namespace MIDIRouterApp
{
template <typename T>
using OwnedVector = std::vector<std::unique_ptr<T>>;

using JSON = nlohmann::json;
//To save some typing, we're gonna import a few commonly used juce classes
//into our namespace
using juce::Colour;
using juce::Component;
using juce::Graphics;
using juce::String;
} // namespace GuiApp