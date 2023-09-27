#pragma once
#include <QThread>
#include "gui/model/subtitle_model.h"

namespace sore
{
    class SubtitleWorker : public QThread
    {
        Q_OBJECT

    public:
        explicit SubtitleWorker(QObject* parent = nullptr, SubtitleModel* model = nullptr)
            : QThread(parent)
            , m_Subtitle(model)
        {
        }

        void setFilepath(const QString& filepath)
        {
            m_Filepath = filepath;
        }

        void run() override
        {
            if(!m_Filepath.isEmpty())
                m_Subtitle->populateData(m_Filepath);
        }

    private:
        SubtitleModel* m_Subtitle = nullptr;
        QString m_Filepath;
    };
}