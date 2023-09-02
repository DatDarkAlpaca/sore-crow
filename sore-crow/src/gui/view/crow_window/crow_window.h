#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_crow_window.h"
#include "core/project/project_data.h"

#include "gui/widgets/episode_widget.h"

#include "gui/objects/crow_media_handler.h"
#include "gui/objects/player_controls_media_events.h"

namespace sore
{
    class CrowWindow : public QMainWindow
    {
        friend class MainController;
        Q_OBJECT

    public:
        CrowWindow(QWidget* parent = nullptr);
        
        ~CrowWindow() = default;

    public:
        void updateData(const ProjectData& data);

        void clearData();

    // Docks:
    private:
        void onShowEpisodeListDock();

        void onShowSubtitleViewerDock();

    // Events:
    private:
        void onEpisodeClicked();

        void onPreviousButtonClick();

        void onNextButtonClick();

    // Actions:
    private:
        void toggleAudioTrackAction(bool value);

        void populateAudioTrackAction();

        void populateAudioDevice();

    private:
        CrowMediaHandler* m_MediaHandler = nullptr;
        PlayerControlsMediaEvents* m_PlayerControlsEvents = nullptr;

    private:
        Ui::CrowWindow ui;
    };
}