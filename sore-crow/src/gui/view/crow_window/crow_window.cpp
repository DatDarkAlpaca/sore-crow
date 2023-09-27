#include "pch.h"
#include "crow_window.h"

#include "core/handlers.h"
#include "utils/action_utils.h"
#include "core/project/project_utils.h"
#include "gui/model/subtitle_delegate.h"
#include "gui/view/preferences/preferences_dialog.h"

namespace sore
{
	CrowWindow::CrowWindow(QWidget* parent)
		: QMainWindow(parent)
	{
		ui.setupUi(this);
		setWindowTitle(windowTitle() + " v" + ProjectVersion);

		// Widgets:
		m_CrowSubtitles = new CrowSubtitles(ui.videoPlayer);

		// Model:
		ui.episodeList->setModel(&m_EpisodeModel);
		ui.subtitleList->setModel(&m_SubtitleModel);
		ui.subtitleList->setItemDelegate(new SubtitleDelegate(ui.subtitleList));

		// Workers:
		auto* mpvHandle = ui.videoPlayer->mpvHandle();
		m_TrackWorker = new MPVTrackWorker(this, mpvHandle);
		m_AudioDeviceWorker = new MPVAudioDeviceWorker(this, mpvHandle);
		m_Worker = new SubtitleWorker(this, &m_SubtitleModel);

		connectEpisodeListSignals();
		connectEpisodeManagerSignals();
		connectPlayerControlSignals();
		connectActionSignals();
		connectSubtitleSignals();
		connectHandlerSignals();

		// Handlers:
		StylesheetHandler::initializePlayerStyles();
	}

	void CrowWindow::loadProjectData(const ProjectData& projectData)
	{
		show();
		m_ProjectData = projectData;

		// Initial Settings:
		ui.videoPlayer->controls->disableControls();
		ui.videoPlayer->clearPlaylist();
		ui.videoPlayer->pause();
		m_FirstSubtitlePopulateAction = true;

		// Volume:
		ui.videoPlayer->controls->setVolumeMaximum(ui.videoPlayer->volumeMax());
		ui.videoPlayer->setVolume(ui.videoPlayer->controls->currentVolume());

		// Episode Model:
		m_EpisodeModel.clear();
		m_SubtitleModel.clear();
		m_EpisodeModel.populateData(projectData.mediaData);

		// Load Playlist:
		bool isFirst = true;
		for (const auto& episodeData : projectData.mediaData.episodeData)
		{
			if (isFirst)
			{
				ui.videoPlayer->openMedia(FilesystemHandler::getMediaFilepath(episodeData.filepath), false);
				isFirst = false;
				continue;
			}

			ui.videoPlayer->openMedia(FilesystemHandler::getMediaFilepath(episodeData.filepath), true);
		}
	}

	void CrowWindow::onNewProjectAction()
	{
		auto projectData = createProjectFromDialog();
		if (!projectData.has_value())
			return;

		loadProjectData(projectData.value());
	}

	void CrowWindow::onOpenProjectAction()
	{
		auto projectData = openProjectFromDialog();
		if (!projectData.has_value())
			return;

		loadProjectData(projectData.value());
	}

	void CrowWindow::onPreferencesAction()
	{
		PreferencesDialog preferencesDialog;
		preferencesDialog.exec();
	}

	void CrowWindow::connectEpisodeListSignals()
	{
		// Episode List:
		connect(ui.episodeList->selectionModel(), &QItemSelectionModel::currentChanged, [&](const QModelIndex& current, const QModelIndex& previous) {
			int episodeIndex = m_EpisodeModel.data(current, EpisodeModel::Roles::IndexRole).toInt();
			
			// Play Episode:
			ui.videoPlayer->playlistPlay(episodeIndex);
			ui.videoPlayer->controls->enableControls();

			m_SubtitleModel.clear();

			// Actions:
			setAudioTrackAction(true);
			setSubtitleTrackAction(true);
			setSecondarySubtitleTrackAction(true);
			setExternalSubtitleAction(true);
		});

		// Player Controls:
		connect(ui.videoPlayer->controls, &PlayerControls::previousBtnClicked, [&]() {
			auto currentIndex = ui.episodeList->currentIndex();

			int episodeIndex = m_EpisodeModel.data(currentIndex, EpisodeModel::Roles::IndexRole).toInt();
			int nextIndex = --episodeIndex;

			if (nextIndex < 0)
				return;

			ui.episodeList->setCurrentIndex(m_EpisodeModel.index(episodeIndex, 0));
		});

		connect(ui.videoPlayer->controls, &PlayerControls::nextBtnClicked, [&]() {
			auto currentIndex = ui.episodeList->currentIndex();
			auto rowCount = m_EpisodeModel.rowCount();

			int episodeIndex = m_EpisodeModel.data(currentIndex, EpisodeModel::Roles::IndexRole).toInt();
			int nextIndex = ++episodeIndex;

			if (nextIndex >= rowCount)
				return;

			ui.episodeList->setCurrentIndex(m_EpisodeModel.index(episodeIndex, 0));
		});
	}

	void CrowWindow::connectEpisodeManagerSignals()
	{
		connect(ui.episodeManager, &EpisodeManager::episodesAddClicked, [&](const QStringList& episodeList) {
			for (const auto& episode : episodeList)
				m_ProjectData.addMedia(episode);

			m_ProjectData.save(m_ProjectData.filepath);

			// Refresh:
			m_EpisodeModel.populateData(m_ProjectData.mediaData);
		});

		connect(ui.episodeManager, &EpisodeManager::episodeRemoveClicked, [&]() {
			auto currentIndex = ui.episodeList->currentIndex();
			auto id = currentIndex.data(EpisodeModel::Roles::IDRole).toUuid();

			m_ProjectData.removeMedia(id);
			m_ProjectData.save(m_ProjectData.filepath);

			// Refresh:
			m_EpisodeModel.populateData(m_ProjectData.mediaData);
		});
	}

	void CrowWindow::connectPlayerControlSignals()
	{
		// Video Controls:
		connect(ui.videoPlayer, &CrowPlayer::playingStatusChanged, ui.videoPlayer->controls, &PlayerControls::setPlaying);
		connect(ui.videoPlayer, &CrowPlayer::positionChanged, ui.videoPlayer->controls, &PlayerControls::trySetPosition);
		connect(ui.videoPlayer, &CrowPlayer::durationChanged, ui.videoPlayer->controls, &PlayerControls::setDuration);
		
		// Player Controls:
		connect(ui.videoPlayer->controls, &PlayerControls::previousBtnClicked, ui.videoPlayer, &CrowPlayer::playlistPrev);
		connect(ui.videoPlayer->controls, &PlayerControls::playBtnClicked, ui.videoPlayer, &CrowPlayer::togglePlay);
		connect(ui.videoPlayer->controls, &PlayerControls::stopBtnClicked, ui.videoPlayer, &CrowPlayer::stop);
		connect(ui.videoPlayer->controls, &PlayerControls::nextBtnClicked, ui.videoPlayer, &CrowPlayer::playlistNext);
		connect(ui.videoPlayer->controls, &PlayerControls::volumeBtnClicked, ui.videoPlayer, &CrowPlayer::setVolume);
		
		connect(ui.videoPlayer->controls, &PlayerControls::repeatBtnClicked, [&]() {
			auto currentIndex = ui.subtitleList->currentIndex();
			if (!currentIndex.isValid())
			{
				ui.videoPlayer->controls->setRepeatChecked(false);
				ui.videoPlayer->setRepeat(false);
				return;
			}

			double start = m_SubtitleModel.data(currentIndex, SubtitleModel::Roles::StartRole).toDouble();
			double end = m_SubtitleModel.data(currentIndex, SubtitleModel::Roles::EndRole).toDouble();

			ui.videoPlayer->toggleRepeat();
			ui.videoPlayer->setRepeatInterval(start, end);
		});

		connect(ui.videoPlayer->controls, &PlayerControls::positionChanged, ui.videoPlayer, &CrowPlayer::seekAbsolute);
		connect(ui.videoPlayer->controls, &PlayerControls::volumeChanged, ui.videoPlayer, &CrowPlayer::setVolume);
	}
	
	void CrowWindow::connectActionSignals()
	{
		// File:
		connect(ui.actionNewProject, &QAction::triggered, this, &CrowWindow::onNewProjectAction);
		connect(ui.actionOpenProject, &QAction::triggered, this, &CrowWindow::onOpenProjectAction);
		connect(ui.actionPreferences, &QAction::triggered, this, &CrowWindow::onPreferencesAction);

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

		connect(ui.actionAddExternalTrack, &QAction::triggered, this, &CrowWindow::onExternalTrackTriggered);

		// Track Worker:
		connect(m_TrackWorker, &MPVTrackWorker::jobFinished, this, &CrowWindow::populateAudioTracks);
		connect(m_TrackWorker, &MPVTrackWorker::jobFinished, this, &CrowWindow::populateSubtitleTracks);
		connect(m_TrackWorker, &MPVTrackWorker::jobFinished, this, &CrowWindow::populateSecondarySubtitleTracks);
		connect(m_AudioDeviceWorker, &MPVAudioDeviceWorker::jobFinished, this, &CrowWindow::populateAudioDevices);
	}

	void CrowWindow::connectSubtitleSignals()
	{
		connect(ui.videoPlayer, &CrowPlayer::positionChanged, [&](double position) {
			if (ui.subtitleList->contextMenuOpen())
				return;

			auto closestIndex = m_SubtitleModel.getClosestSubtitle(position);
			if (!closestIndex.has_value())
				return;

			ui.subtitleList->setCurrentIndex(closestIndex.value());
		});

		connect(ui.subtitleList, &SubtitleListView::jumpedToTimestamp, ui.videoPlayer, &CrowPlayer::seekAbsolute);
	}

	void CrowWindow::connectHandlerSignals()
	{
		connect(StylesheetHandler::instance(), &StylesheetHandler::overrideStylesChanged, 
				ui.videoPlayer, &CrowPlayer::overrideSubtitleStyles);

		connect(StylesheetHandler::instance(), &StylesheetHandler::subtitleStyleChanged,
			ui.videoPlayer, &CrowPlayer::setSubtitleStyle);
	}

	void CrowWindow::setAudioTrackAction(bool enabled)
	{
		ui.menuAudioTrack->setEnabled(enabled);
	}

	void CrowWindow::setSubtitleTrackAction(bool enabled)
	{
		ui.menuSubtitleTrack->setEnabled(enabled);
	}

	void CrowWindow::setSecondarySubtitleTrackAction(bool enabled)
	{
		ui.menuSecondarySubtitle->setEnabled(enabled);
	}

	void CrowWindow::setExternalSubtitleAction(bool enabled)
	{
		ui.actionAddExternalTrack->setEnabled(enabled);
	}

	void CrowWindow::populateAudioTracks(const std::vector<Track>& tracks)
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

	void CrowWindow::populateAudioDevices(const std::vector<AudioDevice>& devices)
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

	void CrowWindow::createDisabledSubtitleTrack()
	{
		QAction* action = new QAction(ui.menuSubtitleTrack);
		action->setCheckable(true);
		action->setChecked(true);
		action->setText("Disabled");
		ui.videoPlayer->setSubtitleVisibility(false);

		connect(action, &QAction::triggered, [&, action](bool checked) {
			uncheckAllButOne(ui.menuSubtitleTrack, action);
			ui.videoPlayer->setSubtitleVisibility(false);
		});

		ui.menuSubtitleTrack->addAction(action);
	}

	void CrowWindow::createDisabledSecondarySubtitleTrack()
	{
		QAction* action = new QAction(ui.menuSecondarySubtitle);
		action->setCheckable(true);
		action->setChecked(true);
		action->setText("Disabled");
		ui.videoPlayer->setSecondarySubtitleVisibility(false);

		connect(action, &QAction::triggered, [&, action](bool checked) {
			uncheckAllButOne(ui.menuSecondarySubtitle, action);
			ui.videoPlayer->setSecondarySubtitleVisibility(false);
		});

		ui.menuSecondarySubtitle->addAction(action);
	}

	void CrowWindow::populateSubtitleTracks(const std::vector<Track>& tracks)
	{
		ui.menuSubtitleTrack->clear();

		createDisabledSubtitleTrack();

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

	void CrowWindow::populateSecondarySubtitleTracks(const std::vector<Track>& tracks)
	{
		ui.menuSecondarySubtitle->clear();

		createDisabledSecondarySubtitleTrack();

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

	void CrowWindow::onExternalTrackTriggered()
	{
		QString filepath = openSubtitleTrackDialog();
		if (filepath.isEmpty())
			return;

		QFileInfo fileInfo(filepath);
		QString subtitleName = fileInfo.baseName();

		// Adds external subtitle & set model:
		ui.videoPlayer->addExternalSubtitle(filepath);
		m_SubtitleModel.populateData(filepath);

		m_FirstSubtitlePopulateAction = false;
	}

	void CrowWindow::onAudioTrackTriggered(QAction* action, const Track& track)
	{
		uncheckAllButOne(ui.menuAudioTrack, action);
		ui.videoPlayer->setAudioTrack(track.id);
	}

	void CrowWindow::onAudioDeviceTriggered(QAction* action, const AudioDevice& device)
	{
		uncheckAllButOne(ui.menuAudioDevice, action);
		ui.videoPlayer->setAudioDevice(device.name);
	}
	
	void CrowWindow::onSubtitleTrackTriggered(QAction* action, const Track& track)
	{
		uncheckAllButOne(ui.menuSubtitleTrack, action);
		ui.videoPlayer->setSubtitleTrack(track.id);
		
		m_Worker->setFilepath(track.externalFilename);
		m_Worker->run();
	}
	
	void CrowWindow::onSecondarySubtitleTrackTriggered(QAction* action, const Track& track)
	{
		uncheckAllButOne(ui.menuSecondarySubtitle, action);
		ui.videoPlayer->setSecondarySubtitleTrack(track.id);
	}
}