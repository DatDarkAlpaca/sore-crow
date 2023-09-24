#pragma once
#include "mpv_node.h"

namespace sore
{
    struct AudioDevice
    {
        QString name;
        QString description;
    };

    static inline AudioDevice parseAudioDevice(const QVariantMap& deviceMap)
    {
        AudioDevice device;

        device.name = deviceMap.value("name", "").toString();
        device.description = deviceMap.value("description", "").toString();

        return device;
    }

    inline std::vector<AudioDevice> getAudioDevices(mpv_handle* handle)
    {
        std::vector<AudioDevice> devices;
        QVariantList trackList = mpv::getProperty(handle, "audio-device-list").toList();

        for (const auto& internalTrack : trackList)
        {
            QVariantMap deviceMap = internalTrack.toMap();
            devices.push_back(parseAudioDevice(deviceMap));
        }

        return devices;
    }
}