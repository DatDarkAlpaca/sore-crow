#include "pch.h"
#include "crow_window.h"
#include "core/handlers.h"
#include "gui/model/subtitle_delegate.h"

namespace sore
{
	CrowWindow::CrowWindow(QWidget* parent)
		: QMainWindow(parent)
	{
		ui.setupUi(this);
		setupWindowTitles();

		// Widgets:
		m_Actions = new CrowWindowActions(this);

		// Model:
		ui.episodeList->setModel(&m_EpisodeModel);
		ui.subtitleList->setModel(&m_SubtitleModel);
		ui.subtitleList->setItemDelegate(new SubtitleDelegate(ui.subtitleList));

		// Signals:
		connectEpisodeListSignals();
		connectEpisodeManagerSignals();
		connectPlayerControlSignals();
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
		m_Actions->reset();

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

	void CrowWindow::connectEpisodeListSignals()
	{
		// Episode List:
		connect(ui.episodeList->selectionModel(), &QItemSelectionModel::currentChanged, [&](const QModelIndex& current, const QModelIndex& previous) {
			int episodeIndex = m_EpisodeModel.data(current, EpisodeModel::Roles::IndexRole).toInt();
			
			// Play Episode:
			ui.videoPlayer->playlistPlay(episodeIndex);
			ui.videoPlayer->controls->enableControls();
			m_Actions->reset();

			m_SubtitleModel.clear();

			// Actions:
			m_Actions->enableTrackActions();
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
			{
				m_ProjectData.addMedia(episode);
				ui.videoPlayer->openMedia(episode, true);
			}

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
	
	void CrowWindow::connectSubtitleSignals()
	{
		// Subtitle List:
		connect(ui.videoPlayer, &CrowPlayer::positionChanged, [&](double position) {
			if (ui.subtitleList->contextMenuOpen())
				return;

			auto closestIndex = m_SubtitleModel.getClosestSubtitle(position);
			if (!closestIndex.has_value())
				return;

			ui.subtitleList->setCurrentIndex(closestIndex.value());
		});

		// Selectable Subtitles:
		connect(ui.videoPlayer, &CrowPlayer::subtitleChanged, [&](const QString& text, double, double, double) {
			ui.videoPlayer->subtitles->setPrimarySubtitle(text);
		});

		connect(ui.videoPlayer, &CrowPlayer::subtitleChangedSecondary, [&](const QString& text, double, double, double) {
			ui.videoPlayer->subtitles->setSecondarySubtitle(text);
		});

		connect(ui.subtitleList, &SubtitleListView::jumpedToTimestamp, ui.videoPlayer, &CrowPlayer::seekAbsolute);
	}

	void CrowWindow::connectHandlerSignals()
	{
		// Video Player:
		connect(StylesheetHandler::instance(), &StylesheetHandler::overrideStylesChanged, 
			ui.videoPlayer, &CrowPlayer::overrideSubtitleStyles);

		connect(StylesheetHandler::instance(), &StylesheetHandler::overrideSelectableSubtiles, [&](bool enabledDefault) {
			ui.videoPlayer->setSubtitleVisibility(!enabledDefault);
			ui.videoPlayer->setSecondarySubtitleVisibility(!enabledDefault);
		});

		connect(StylesheetHandler::instance(), &StylesheetHandler::subtitleStyleChanged,
			ui.videoPlayer, &CrowPlayer::setSubtitleStyle);

		// Subtitles:
		connect(StylesheetHandler::instance(), &StylesheetHandler::overrideSelectableSubtiles,
			ui.videoPlayer->subtitles, &SelectableSubtitles::toggleSubtitles);

		connect(StylesheetHandler::instance(), &StylesheetHandler::overrideSelectableSubtiles, [&](bool enabledDefault) {
			ui.videoPlayer->subtitles->toggleSubtitles(enabledDefault);
		});

		connect(StylesheetHandler::instance(), &StylesheetHandler::subtitleStyleChanged,
			ui.videoPlayer->subtitles, &SelectableSubtitles::setSubtitleStyles);
	}

	void CrowWindow::setupWindowTitles()
	{
		QString title = windowTitle() + " v" + ProjectVersion;
		setWindowTitle(title);
		ui.videoPlayer->setWindowTitle(title);
	}
}