#pragma once
#include <QtWidgets/QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>

#include "ui_crow_window.h"
#include "gui/view/episode_widget.h"
#include "core/project/project_data.h"

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

    // Episode Handlers:
    private:
        void insertProjectEpisodes(const ProjectData& data);

        void handleEpisodeClicked(EpisodeWidget* episodeWidget, const std::string& episodeFilepath);

        void addEpisodeToDock(EpisodeWidget* widget);

    // Video:
    private:
        void setVideo(const std::string& videoFilepath);

        void setVideoSliderMaximum(long long videoDuration);

        void handleVideoPositionChange();

        inline bool hasVideoSource();

    // Audio:
    private:
        void handleVolumePositionChange();

    // Controllers:
        void setCurrentDurationLabel(long long duration);

        void setTotalDurationLabel(long long duration);

    // Buttons:
    private:
        void togglePlayButtonIcon(bool togglePlayingIcon);

        void handlePlayButtonClicked();

        void handleStopButtonClick();

        void handlePreviousButtonClick();

        void handleNextButtonClick();

    private:
        void setupPlayer();

        void setupAudioSlider();

        void setupIcons();

    private:
        QMediaPlayer* m_MediaPlayer = nullptr;
        QAudioOutput* m_AudioOutput = nullptr;

    private:
        Ui::CrowWindow ui;
    };
}