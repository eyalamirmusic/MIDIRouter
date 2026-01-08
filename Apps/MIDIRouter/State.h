#pragma once

#include "ConnectionDescription.h"

namespace MIDIRouterApp
{

inline juce::File getPrefsFile()
{
    return juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
        .getChildFile("MIDI Router")
        .getChildFile("Prefs.json");
}

struct State
{
    State() { loadFromFile(); }

    ~State() { saveToFile(); }

    void createConnection(const MidiDeviceInfo& inputID)
    {
        if (!hasConnection(inputID))
        {
            connections.create();
            connections.back().input = inputID;
        }
    }

    void saveToFile()
    {
        auto file = getPrefsFile();

        if (file.existsAsFile())
            file.deleteFile();

        file.create();

        if (auto stream = file.createOutputStream())
        {
            auto json = String(save().dump(4));
            stream->writeText(json, false, false, nullptr);
            stream->flush();
        }
    }

    void loadFromFile()
    {
        auto file = getPrefsFile();

        if (file.exists())
        {
            auto contents = file.loadFileAsString();

            try
            {
                auto parsed = JSON::parse(contents.toStdString());
                load(parsed);
            }
            catch (...)
            {
            }
        }
    }

    JSON save() const
    {
        auto res = JSON();

        for (auto& connection: connections)
            res.emplace_back(connection.save());

        return res;
    }

    void load(const JSON& json)
    {
        connections.clear();

        for (auto& element: json)
        {
            connections.create();
            connections.back().load(element);
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

    std::function<void()> rebuildConnections = []{};
    Vector<ConnectionDescription> connections;
};

} // namespace MIDIRouterApp