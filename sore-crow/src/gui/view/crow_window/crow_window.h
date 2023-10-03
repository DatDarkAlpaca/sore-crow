#pragma once
#include "ui_crow_window.h"
#include "crow_window_actions.h"
#include "gui/model/episode_model.h"
#include "gui/model/subtitle_model.h"

namespace sore
{
    class CrowWindow : public QMainWindow
    {
        Q_OBJECT
        friend class CrowWindowActions;

    public:
        CrowWindow(QWidget* parent = nullptr);

        ~CrowWindow() = default;

    public:
        void loadProjectData(const ProjectData& projectData);

    private:
        void connectEpisodeListSignals();

        void connectEpisodeManagerSignals();

        void connectPlayerControlSignals();

        void connectSubtitleSignals();

        void connectHandlerSignals();

    private:
        void setupWindowTitles();

    private:
        Ui::CrowWindow ui;

    private:
        EpisodeModel m_EpisodeModel;
        SubtitleModel m_SubtitleModel;
        ProjectData m_ProjectData;

    private:
        CrowWindowActions* m_Actions;
    };
}