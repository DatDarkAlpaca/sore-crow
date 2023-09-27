#include "pch.h"
#include <QObject>
#include "crow_window.h"
#include "utils/action_utils.h"
#include "crow_window_actions.h"
#include "core/project/project_utils.h"
#include "gui/view/preferences/preferences_dialog.h"

namespace sore
{
	CrowWindowActions::CrowWindowActions(CrowWindow* parent)
		: m_CrowWindow(parent)
		, ui(m_CrowWindow->ui)
	{
		setupWorkers();
		connectActionSignals();
	}

	void CrowWindowActions::enableTrackActions()
	{
		ui.menuAudioTrack->setEnabled(true);
		ui.menuSubtitleTrack->setEnabled(true);
		ui.menuSecondarySubtitle->setEnabled(true);
		ui.actionAddExternalTrack->setEnabled(true);
	}

	void CrowWindowActions::reset()
	{
		m_FirstSubtitlePopulateAction = true;
	}

	void CrowWindowActions::setupWorkers()
	{
		auto* mpvHandle = ui.videoPlayer->mpvHandle();

		m_TrackWorker = new MPVTrackWorker(m_CrowWindow, mpvHandle);
		m_AudioDeviceWorker = new MPVAudioDeviceWorker(m_CrowWindow, mpvHandle);
		m_Worker = new SubtitleWorker(m_CrowWindow, &m_CrowWindow->m_SubtitleModel);
	}

	void CrowWindowActions::connectActionSignals()
	{
		// File:
		connect(ui.actionNewProject, &QAction::triggered, this, &CrowWindowActions::onNewProjectAction);
		connect(ui.actionOpenProject, &QAction::triggered, this, &CrowWindowActions::onOpenProjectAction);
		connect(ui.actionPreferences, &QAction::triggered, this, &CrowWindowActions::onPreferencesAction);

		// Audio | Video | Sub Tracks:
		connect(ui.videoPlayer, &CrowPlayer::trackListChanged, [&](long long trackAmount) {
			if (trackAmount <= 0)
				return;

			m_TrackWorker->start();
		});

		connect(ui.videoPlayer, &CrowPlayer::audioDevicesChanged, [&](int64_t deviceAmount) {
			if (deviceAmount <= 0)
				return;

			m_AudioDeviceWorker->start();
		});

		connect(ui.actionAddExternalTrack, &QAction::triggered, this, &CrowWindowActions::onExternalTrackTriggered);

		// Track Worker:
		connect(m_TrackWorker, &MPVTrackWorker::jobFinished, this, &CrowWindowActions::populateAudioTracks);
		connect(m_TrackWorker, &MPVTrackWorker::jobFinished, this, &CrowWindowActions::populateSubtitleTracks);
		connect(m_TrackWorker, &MPVTrackWorker::jobFinished, this, &CrowWindowActions::populateSecondarySubtitleTracks);
		connect(m_AudioDeviceWorker, &MPVAudioDeviceWorker::jobFinished, this, &CrowWindowActions::populateAudioDevices);
	}

	void CrowWindowActions::onNewProjectAction()
	{
		auto projectData = createProjectFromDialog();
		if (!projectData.has_value())
			return;

		m_CrowWindow->loadProjectData(projectData.value());
	}

	void CrowWindowActions::onOpenProjectAction()
	{
		auto projectData = openProjectFromDialog();
		if (!projectData.has_value())
			return;

		m_CrowWindow->loadProjectData(projectData.value());
	}

	void CrowWindowActions::onPreferencesAction()
	{
		PreferencesDialog preferencesDialog;
		preferencesDialog.exec();
	}

	void CrowWindowActions::onExternalTrackTriggered()
	{
		QString filepath = openSubtitleTrackDialog();
		if (filepath.isEmpty())
			return;

		QFileInfo fileInfo(filepath);
		QString subtitleName = fileInfo.baseName();

		// Adds external subtitle & set model:
		ui.videoPlayer->addExternalSubtitle(filepath);
		m_CrowWindow->m_SubtitleModel.populateData(filepath);

		m_FirstSubtitlePopulateAction = false;
	}

	void CrowWindowActions::populateAudioTracks(const std::vector<Track>& tracks)
	{
		ui.menuAudioTrack->clear();

		bool first = true;
		size_t index = 0;
		for (const auto& track : tracks)
		{
			if (track.type != "audio")
				continue;

			QAction* action = new QAction(ui.menuAudioTrack);
			action->setCheckable(true);
			action->setText(getBestTrackTitle(track, index));

			// Defaults the first audio track:
			if (first)
			{
				first = false;
				action->setChecked(true);
				ui.videoPlayer->setAudioTrack(track.id);
			}

			connect(action, &QAction::triggered, [&, track, action](bool checked) {
				onAudioTrackTriggered(action, track);
			});

			++index;
			ui.menuAudioTrack->addAction(action);
		}
	}

	void CrowWindowActions::populateAudioDevices(const std::vector<AudioDevice>& devices)
	{
		ui.menuAudioDevice->clear();

		bool first = true;
		for (const auto& device : devices)
		{
			QAction* action = new QAction(ui.menuAudioDevice);
			action->setCheckable(true);
			action->setText(device.description);

			// Defaults the first audio track:
			if (first)
			{
				first = false;
				action->setChecked(true);
				ui.videoPlayer->setAudioDevice(device.name);
			}

			connect(action, &QAction::triggered, [&, device, action](bool checked) {
				onAudioDeviceTriggered(action, device);
			});

			ui.menuAudioDevice->addAction(action);
		}
	}
	
	void CrowWindowActions::populateSubtitleTracks(const std::vector<Track>& tracks)
	{
		ui.menuSubtitleTrack->clear();

		createDisabledSubtitleTrack(ui.menuSubtitleTrack, false);

		QAction* lastAction = nullptr;
		Track lastTrack;
		size_t index = 0;
		for (const auto& track : tracks)
		{
			if (track.type != "sub")
				continue;

			QAction* action = new QAction(ui.menuSubtitleTrack);
			action->setCheckable(true);
			action->setText(getBestTrackTitle(track, index));

			connect(action, &QAction::triggered, [&, track, action](bool checked) {
				onSubtitleTrackTriggered(action, track);
			});

			lastAction = action;
			lastTrack = track;

			++index;
			ui.menuSubtitleTrack->addAction(action);
		}

		if (!m_FirstSubtitlePopulateAction && lastAction)
		{
			uncheckAll(ui.menuSubtitleTrack);
			lastAction->setChecked(true);
			ui.videoPlayer->setSubtitleTrack(lastTrack.id);
		}
	}

	void CrowWindowActions::populateSecondarySubtitleTracks(const std::vector<Track>& tracks)
	{
		ui.menuSecondarySubtitle->clear();

		createDisabledSubtitleTrack(ui.menuSecondarySubtitle, true);

		QAction* lastAction = nullptr;
		Track lastTrack;
		size_t index = 0;
		for (const auto& track : tracks)
		{
			if (track.type != "sub")
				continue;

			QAction* action = new QAction(ui.menuSecondarySubtitle);
			action->setCheckable(true);
			action->setText(getBestTrackTitle(track, index));

			connect(action, &QAction::triggered, [&, track, action](bool checked) {
				onSecondarySubtitleTrackTriggered(action, track);
				});

			lastAction = action;
			lastTrack = track;

			++index;
			ui.menuSecondarySubtitle->addAction(action);
		}
	}

	void CrowWindowActions::onAudioTrackTriggered(QAction* action, const Track& track)
	{
		uncheckAllButOne(ui.menuAudioTrack, action);
		ui.videoPlayer->setAudioTrack(track.id);
	}

	void CrowWindowActions::onAudioDeviceTriggered(QAction* action, const AudioDevice& device)
	{
		uncheckAllButOne(ui.menuAudioDevice, action);
		ui.videoPlayer->setAudioDevice(device.name);
	}

	void CrowWindowActions::onSubtitleTrackTriggered(QAction* action, const Track& track)
	{
		uncheckAllButOne(ui.menuSubtitleTrack, action);
		ui.videoPlayer->setSubtitleTrack(track.id);

		m_Worker->setFilepath(track.externalFilename);
		m_Worker->run();
	}

	void CrowWindowActions::onSecondarySubtitleTrackTriggered(QAction* action, const Track& track)
	{
		uncheckAllButOne(ui.menuSecondarySubtitle, action);
		ui.videoPlayer->setSecondarySubtitleTrack(track.id);
	}

	void CrowWindowActions::createDisabledSubtitleTrack(QMenu* menu, bool isSecondary)
	{
		QAction* action = new QAction(menu);
		action->setCheckable(true);
		action->setChecked(true);
		action->setText("Disabled");

		auto setVisibility = [&](bool enabled) {
			if (!isSecondary)
				ui.videoPlayer->setSubtitleVisibility(false);
			else
				ui.videoPlayer->setSecondarySubtitleVisibility(false);
		};

		setVisibility(false);

		connect(action, &QAction::triggered, [&, action](bool checked) {
			uncheckAllButOne(menu, action);
			setVisibility(false);
		});

		menu->addAction(action);
	}
}