#pragma once

#include "State.h"
#include "LiveConnection.h"

namespace MIDIRouterApp
{
struct MIDIRouter
    : juce::MidiInputCallback
    , juce::Timer
{
    MIDIRouter(State& stateToUse)
        : state(stateToUse)
    {
        startTimerHz(1);
        update();
    }

    ~MIDIRouter() override { stopTimer(); }

    void timerCallback() override { update(); }

    bool isStateInvalidated() const
    {
        if (state.connections.size() != liveConnections.size())
            return true;

        for (int index = 0; index < state.connections.size(); ++index)
        {
            if (!liveConnections[index]->isMatching(state.connections[index]))
                return true;
        }

        return false;
    }

    void update()
    {
        auto sl = EA::Locks::ScopedSpinLock (lock);

        if (isStateInvalidated())
        {
            liveConnections.clear();

            for (auto& connection: state.connections)
            {
                if (auto newConnection = createConnection(connection, *this))
                    liveConnections.emplace_back(std::move(newConnection));
            }
        }
    }

    void handleIncomingMidiMessage(MidiInput* input,
                                   const juce::MidiMessage& message) override
    {
        auto sl = EA::Locks::ScopedSpinLock (lock);

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
    EA::Locks::RecursiveSpinLock lock;
    OwnedVector<LiveConnection> liveConnections;
};
} // namespace MIDIRouterApp