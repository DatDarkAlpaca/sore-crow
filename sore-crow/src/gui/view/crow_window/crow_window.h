#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_crow_window.h"

#include "gui/objects/subtitle_handler.h"
#include "gui/objects/crow_media_handler.h"
#include "core/project/project_data.h"
#include "gui/widgets/episode_widget/episode_widget.h"
#include "gui/model/subtitle_model.h"

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

    private:
        void handleActions();

    protected:
        void resizeEvent(QResizeEvent* event) override;

    // Docks:
    private slots:
        void onShowEpisodeListDock();

        void onShowSubtitleViewerDock();

    // Video Player:
    private slots:
        void onVideoPositionPositionChanged();

        void onVideoPlayerSliderChanged();

        void onVolumeSliderChanged();

        void onSectionRepeatStopped();

        void onPlayButtonClicked();

        void onStopButtonClicked();

        void onVideoRepeatClicked();

        void onVolumeButtonClicked();

    // Episodes:
    private slots:
        void onEpisodeClicked();

        void onPreviousButtonClick();

        void onNextButtonClick();

        void onRepeatButtonClick();

    // Subtitles:
    private slots:
        void onSubtitleClicked();

    // Actions:
    private:
        void toggleAudioTrackAction(bool value);

        void toggleSubtitleTrackAction(bool value);

        void toggleExternalSubtitleAction(bool value);

        void populateAudioTrackAction();

        void populateAudioDeviceAction();

        void populateSubtitleTrackAction();

        void onExternalSubtitleAction();

    private:
        CrowMediaHandler* m_MediaHandler = nullptr;
        SubtitleHandler* m_SubtitleHandler = nullptr;

    private:
        SubtitleModel m_SubtitleModel;

        Ui::CrowWindow ui;
    };
}