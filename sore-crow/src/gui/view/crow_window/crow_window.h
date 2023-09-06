#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_crow_window.h"

#include "core/project/project_data.h"
#include "gui/objects/crow_media_handler.h"
#include "gui/widgets/episode_widget/episode_widget.h"

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

    // Video Player:
    private:
        void onVideoPositionPositionChanged();

        void onVideoPlayerSliderChanged();

        void onPlayButtonClicked();

        void onStopButtonClicked();

        void onVolumeButtonClicked();

    // Episodes:
    private:
        void onEpisodeClicked();

        void onPreviousButtonClick();

        void onNextButtonClick();

    // Actions:
    private:
        void toggleAudioTrackAction(bool value);

        void toggleSubtitleTrackAction(bool value);

        void toggleExternalSubtitleAction(bool value);

        void populateAudioTrackAction();

        void populateAudioDeviceAction();

        void populateSubtitleTrackAction();

    private:
        CrowMediaHandler* m_MediaHandler = nullptr;

    private:
        Ui::CrowWindow ui;
    };
}