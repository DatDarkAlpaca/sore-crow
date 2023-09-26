#pragma once
#include "ui_crow_window.h"
#include "gui/model/episode_model.h"
#include "gui/model/subtitle_model.h"
#include "core/worker/workers.h"
#include "gui/widgets/crow_subtitles/crow_subtitles.h"

namespace sore
{
    class CrowWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        CrowWindow(QWidget* parent = nullptr);

        ~CrowWindow() = default;

    public:
        void loadProjectData(const ProjectData& projectData);

    private:
        void onNewProjectAction();

        void onOpenProjectAction();

        void onPreferencesAction();

    private:
        void connectEpisodeListSignals();

        void connectPlayerControlSignals();

        void connectActionSignals();

        void connectSubtitleSignals();

        void connectHandlerSignals();

    // Track Utils:
    private:
        void setAudioTrackAction(bool enabled);

        void setSubtitleTrackAction(bool enabled);

        void setSecondarySubtitleTrackAction(bool enabled);

        void setExternalSubtitleAction(bool enabled);

        void populateAudioTracks(const std::vector<Track>& tracks);

        void populateAudioDevices(const std::vector<AudioDevice>& devices);

        void createDisabledSubtitleTrack();

        void createDisabledSecondarySubtitleTrack();

        void populateSubtitleTracks(const std::vector<Track>& tracks);

        void populateSecondarySubtitleTracks(const std::vector<Track>& tracks);

    // Track Slots
    private:
        void onExternalTrackTriggered();

        void onAudioTrackTriggered(QAction* action, const Track& track);

        void onAudioDeviceTriggered(QAction* action, const AudioDevice& device);

        void onSubtitleTrackTriggered(QAction* action, const Track& track);
    
        void onSecondarySubtitleTrackTriggered(QAction* action, const Track& track);

    private:
        Ui::CrowWindow ui;

    private:
        CrowSubtitles* m_CrowSubtitles;

    private:
        EpisodeModel m_EpisodeModel;
        SubtitleModel m_SubtitleModel;

    private:
        SubtitleWorker* m_Worker;
        MPVTrackWorker* m_TrackWorker;
        MPVAudioDeviceWorker* m_AudioDeviceWorker;

    private:
        bool m_FirstSubtitlePopulateAction = true;
    };
}