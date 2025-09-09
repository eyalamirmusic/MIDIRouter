#pragma once

#include "CommonHeader.h"

namespace MIDIRouterApp
{
using juce::MidiDeviceInfo;

inline MidiDeviceInfo fromJSON(const JSON& json)
{
    auto res = MidiDeviceInfo();
    res.identifier = json["id"].get<std::string>();
    res.name = json["name"].get<std::string>();

    return res;
}

inline JSON toJSON(const MidiDeviceInfo& info)
{
    auto res = JSON();
    res["id"] = info.identifier.toStdString();
    res["name"] = info.name.toStdString();

    return res;
}

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

    JSON save() const
    {
        auto res = JSON();
        res["input"] = toJSON(input);

        auto& resOutputs = res["outputs"];

        for (auto& output: outputs)
            resOutputs.emplace_back(toJSON(output));

        return res;
    }

    void load(const JSON& json)
    {
        input = fromJSON(json["input"]);

        outputs.clear();

        for (auto& output: json["outputs"])
            outputs.emplace_back(fromJSON(output));
    }

    MidiDeviceInfo input;
    std::vector<MidiDeviceInfo> outputs;
};
}