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
        auto sl = juce::ScopedLock(lock);

        if (isStateInvalidated())
        {
            std::cout << "Invalidated\n";

            liveConnections.clear();

            std::cout << "Cleared connections\n";

            for (auto& connection: state.connections)
            {
                if (auto newConnection = createConnection(connection, *this))
                    liveConnections.emplace_back(std::move(newConnection));
            }
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
}