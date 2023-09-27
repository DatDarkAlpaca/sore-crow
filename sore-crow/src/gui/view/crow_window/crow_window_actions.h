#pragma once
#include "ui_crow_window.h"
#include "gui/worker/workers.h"
#include "core/mpv/utils/mpv_tracks.h"
#include "core/mpv/utils/mpv_audio_devices.h"

namespace sore
{
	class CrowWindow;

	class CrowWindowActions : public QObject
	{
		Q_OBJECT

	public:
		CrowWindowActions(CrowWindow* parent = nullptr);

	public:
		void enableTrackActions();

		void reset();

	private:
		void setupWorkers();

		void connectActionSignals();

	private:
		void onNewProjectAction();

		void onOpenProjectAction();

		void onPreferencesAction();

		void onExternalTrackTriggered();

	private:
		void populateAudioTracks(const std::vector<Track>& tracks);

		void populateAudioDevices(const std::vector<AudioDevice>& devices);

		void populateSubtitleTracks(const std::vector<Track>& tracks);

		void populateSecondarySubtitleTracks(const std::vector<Track>& tracks);

	private:
		void onAudioTrackTriggered(QAction* action, const Track& track);

		void onAudioDeviceTriggered(QAction* action, const AudioDevice& device);

		void onSubtitleTrackTriggered(QAction* action, const Track& track);

		void onSecondarySubtitleTrackTriggered(QAction* action, const Track& track);

	private:
		void createDisabledSubtitleTrack();

		void createDisabledSecondarySubtitleTrack();

	private:
		CrowWindow* m_CrowWindow = nullptr;
		SubtitleWorker* m_Worker = nullptr;
		MPVTrackWorker* m_TrackWorker = nullptr;
		MPVAudioDeviceWorker* m_AudioDeviceWorker = nullptr;

	private:
		bool m_FirstSubtitlePopulateAction = true;
		Ui::CrowWindow& ui;
	};
}