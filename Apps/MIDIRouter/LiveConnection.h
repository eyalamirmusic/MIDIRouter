#pragma once

#include "ConnectionDescription.h"

namespace MIDIRouterApp
{
struct LiveConnection
{
    bool isMatching(const ConnectionDescription& description) const
    {
        if (input->getIdentifier() != description.input.identifier)
            return false;

        if (description.outputs.size() != outputs.size())
            return false;

        for (int index = 0; index < description.outputs.size(); ++index)
        {
            if (outputs[index]->getIdentifier()
                != description.outputs[index].identifier)
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

    OwningPointer<MidiInput> input = nullptr;
    OwnedVector<MidiOutput> outputs;
};

inline OwningPointer<LiveConnection>
    createConnection(const ConnectionDescription& description,
                     juce::MidiInputCallback& cb)
{
    auto newConnection = EA::makeOwned<LiveConnection>();

    auto inputDevices = MidiInput::getAvailableDevices();

    for (auto& device: inputDevices)
    {
        if (device.identifier == description.input.identifier)
        {
            newConnection->input =
                MidiInput::openDevice(description.input.identifier, &cb);
        }
    }

    auto outputDevices = MidiOutput::getAvailableDevices();

    for (auto output: description.outputs)
    {
        for (auto& device: outputDevices)
        {
            if (device.identifier == output.identifier)
            {
                newConnection->outputs.create(
                    MidiOutput::openDevice(output.identifier));
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
} // namespace MIDIRouterApp