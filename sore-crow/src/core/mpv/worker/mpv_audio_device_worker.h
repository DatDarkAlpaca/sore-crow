#pragma once
#include <QThread>
#include "core/mpv/utils/mpv_audio_devices.h"

namespace sore
{
    class MPVAudioDeviceWorker : public QThread
    {
        Q_OBJECT

    public:
        explicit MPVAudioDeviceWorker(QObject* parent = nullptr, mpv_handle* handle = nullptr)
            : QThread(parent)
            , m_Handle(handle)
        {
        }

        void run() override
        {
            auto tracks = getAudioDevices(m_Handle);
            emit jobFinished(tracks);
        }

    signals:
        void jobFinished(const std::vector<AudioDevice>&);

    private:
        mpv_handle* m_Handle;
    };
}