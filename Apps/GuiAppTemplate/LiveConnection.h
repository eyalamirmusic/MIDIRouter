#pragma once

#include "ConnectionDescription.h"

namespace GuiApp
{
struct LiveConnection
{
    bool isMatching(const ConnectionDescription& description) const
    {
        if (input->getIdentifier() != description.input.identifier)
            return false;

        if (description.outputs.size() != outputs.size())
            return false;

        for (size_t index = 0; index < description.outputs.size(); ++index)
        {
            if (outputs[index]->getIdentifier() != description.outputs[index].identifier)
                return false;
        }

        return true;
    }

    bool isValid() const
    {
        if (input == nullptr)
            return false;

        for (auto& output: outputs)
        {
            if (output == nullptr)
                return false;
        }

        return true;
    }

    std::unique_ptr<juce::MidiInput> input = nullptr;
    OwnedVector<juce::MidiOutput> outputs;
};

inline std::unique_ptr<LiveConnection>
    createConnection(const ConnectionDescription& description,
                     juce::MidiInputCallback& cb)
{
    auto newConnection = std::make_unique<LiveConnection>();

    auto inputDevices = juce::MidiInput::getAvailableDevices();

    for (auto& device: inputDevices)
    {
        if (device.identifier == description.input.identifier)
        {
            newConnection->input =
                juce::MidiInput::openDevice(description.input.identifier, &cb);
        }
    }

    auto outputDevices = juce::MidiOutput::getAvailableDevices();

    for (auto output: description.outputs)
    {
        for (auto& device: outputDevices)
        {
            if (device.identifier == output.identifier)
            {
                newConnection->outputs.emplace_back(
                    juce::MidiOutput::openDevice(output.identifier));
            }
        }
    }

    if (newConnection->isValid())
    {
        newConnection->input->start();
        return newConnection;
    }

    std::cout << "invalid connection\n";

    return nullptr;
}
} // namespace GuiApp