#pragma once
#include <QThread>
#include "core/mpv/utils/mpv_tracks.h"

namespace sore
{
    class MPVTrackWorker : public QThread
    {
        Q_OBJECT

    public:
        explicit MPVTrackWorker(QObject* parent = nullptr, mpv_handle* handle = nullptr)
            : QThread(parent)
            , m_Handle(handle)
        {
        }

        void run() override
        {
            auto tracks = getTracks(m_Handle);
            emit jobFinished(tracks);
        }

    signals:
        void jobFinished(const std::vector<Track>&);

    private:
        mpv_handle* m_Handle;
    };
}