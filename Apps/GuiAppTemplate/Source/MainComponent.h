#pragma once

#include "CommonHeader.h"

template <typename T>
using OwnedVector = std::vector<std::unique_ptr<T>>;

namespace GuiApp
{
using juce::MidiDeviceInfo;

struct ConnectionDescription
{
    void connect(const MidiDeviceInfo& info)
    {
        if (!hasOutput(info))
            outputs.emplace_back(info);
    }

    void toggle(const MidiDeviceInfo& info)
    {
        if (hasOutput(info))
            disconnect(info);
        else
            connect(info);
    }

    void disconnect(const MidiDeviceInfo& info)
    {
        auto removed = std::ranges::remove(outputs, info).begin();
        outputs.erase(removed);
    }

    bool hasOutput(const MidiDeviceInfo& info) const
    {
        for (auto& output: outputs)
        {
            if (info == output)
                return true;
        }

        return false;
    }

    MidiDeviceInfo input;
    std::vector<MidiDeviceInfo> outputs;
};

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

    return nullptr;
}

struct State
{
    void createConnection(const MidiDeviceInfo& inputID)
    {
        if (!hasConnection(inputID))
        {
            connections.emplace_back();
            connections.back().input = inputID;
        }
    }

    bool hasConnection(const MidiDeviceInfo& inputID) const
    {
        for (auto& connection: connections)
        {
            if (connection.input == inputID)
                return true;
        }

        return false;
    }

    std::vector<ConnectionDescription> connections;
};

struct MIDIRouter
    : juce::MidiInputCallback
    , juce::Timer
{
    MIDIRouter(State& stateToUse)
        : state(stateToUse)
    {
        update();
        startTimerHz(1);
    }

    ~MIDIRouter() override { stopTimer(); }

    void timerCallback() override { update(); }

    bool isStateInvalidated() const
    {
        if (state.connections.size() != liveConnections.size())
            return true;

        for (size_t index = 0; index < state.connections.size(); ++index)
        {
            if (!liveConnections[index]->isMatching(state.connections[index]))
                return true;
        }

        return false;
    }

    void update()
    {
        if (isStateInvalidated())
        {
            auto sl = juce::ScopedLock(lock);

            liveConnections.clear();

            for (auto& connection: state.connections)
                liveConnections.emplace_back(createConnection(connection, *this));
        }
    }

    void handleIncomingMidiMessage(juce::MidiInput* input,
                                   const juce::MidiMessage& message) override
    {
        auto sl = juce::ScopedLock(lock);

        for (auto& connection: liveConnections)
        {
            if (connection->input.get() == input)
            {
                for (auto& output: connection->outputs)
                    output->sendMessageNow(message);
            }
        }
    }

    State& state;
    juce::CriticalSection lock;
    OwnedVector<LiveConnection> liveConnections;
};
} // namespace GuiApp