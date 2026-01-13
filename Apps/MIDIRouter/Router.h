#pragma once

#include "State.h"
#include "LiveConnection.h"

namespace MIDIRouterApp
{
struct MIDIRouter : juce::Timer
{
    MIDIRouter(State& stateToUse)
        : state(stateToUse)
    {
        startTimerHz(1);
        update();
        state.rebuildConnections = [&] { rebuildConnections(); };
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

    void rebuildConnections()
    {
        auto sl = EA::Locks::ScopedSpinLock(lock);

        liveConnections.clear();

        auto cb = [this](const MIDI::Message& m, const LiveConnection& connection)
        {
            auto localLock = EA::Locks::ScopedSpinLock(lock);

            for (auto& output: connection.outputs)
                output->send(m);
        };

        for (auto& connection: state.connections)
        {
            if (auto newConnection = createConnection(connection, cb))
                liveConnections.emplace_back(std::move(newConnection));
        }
    }

    void update()
    {
        auto sl = EA::Locks::ScopedSpinLock(lock);

        if (isStateInvalidated())
            rebuildConnections();
    }

    State& state;
    EA::Locks::RecursiveSpinLock lock;
    OwnedVector<LiveConnection> liveConnections;
};
} // namespace MIDIRouterApp