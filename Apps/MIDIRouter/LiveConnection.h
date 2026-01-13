#pragma once

#include "ConnectionDescription.h"
#include "RTMIDI/Interface.h"

namespace MIDIRouterApp
{
struct LiveConnection
{
    using ConnectionCB = std::function<void(const MIDI::Message&, LiveConnection&)>;

    bool isMatching(const ConnectionDescription& description) const
    {
        if (input->getName() != description.input)
            return false;

        if (description.outputs.size() != outputs.size())
            return false;

        for (int index = 0; index < description.outputs.size(); ++index)
        {
            if (outputs[index]->getName() != description.outputs[index])
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

    OwningPointer<RTMIDI::InputPort> input;
    OwnedVector<RTMIDI::OutputPort> outputs;
};

inline OwningPointer<RTMIDI::InputPort> createInputPort(const String& name,
                                                        const MIDI::Callback& cb)
{
    auto portList = RTMIDI::getPortList();

    if (auto port = RTMIDI::findFirstContains(portList.inputs, name.toStdString()))
        return EA::makeOwned<RTMIDI::InputPort>(port, cb);

    return nullptr;
}

inline OwningPointer<RTMIDI::OutputPort> createOutputPort(const String& name)
{
    auto portList = RTMIDI::getPortList();

    if (auto port = RTMIDI::findFirstContains(portList.outputs, name.toStdString()))
        return EA::makeOwned<RTMIDI::OutputPort>(port);

    return nullptr;
}

inline OwningPointer<LiveConnection>
    createConnection(const ConnectionDescription& description,
                     const LiveConnection::ConnectionCB& cb)
{
    auto newConnection = EA::makeOwned<LiveConnection>();
    auto c = newConnection.get();

    auto inputCB = [cb, c](const MIDI::Message& m)
    { cb(m, *c); };
    newConnection->input = createInputPort(description.input, inputCB);

    for (auto& output: description.outputs)
        newConnection->outputs.create(createOutputPort(output));

    if (newConnection->isValid())
        return newConnection;

    std::cout << "invalid connection\n";

    return nullptr;
}
} // namespace MIDIRouterApp