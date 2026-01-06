#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <nlohmann/json.hpp>
#include <ea_data_structures/ea_data_structures.h>

namespace MIDIRouterApp
{
using EA::OwnedVector;
using EA::OwningPointer;
using EA::Vector;

using JSON = nlohmann::json;
//To save some typing, we're gonna import a few commonly used juce classes
//into our namespace
using juce::Colour;
using juce::Component;
using juce::Graphics;
using juce::MidiInput;
using juce::MidiOutput;
using juce::String;
} // namespace MIDIRouterApp