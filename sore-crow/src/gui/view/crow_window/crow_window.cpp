#include "pch.h"
#include "crow_window.h"
#include "utils/dialog.h"
#include "core/logger/logger.h"

namespace sore
{
    CrowWindow::CrowWindow(QWidget* parent)
        : QMainWindow(parent)
    {
        ui.setupUi(this);

        m_MediaHandler = new CrowMediaHandler(this);
        m_MediaHandler->setVideoOutput(ui.videoPlayer->videoItem());

        m_SubtitleHandler = new SubtitleHandler(this);

        // Subtitle List:
        ui.subtitleList->setModel(&m_SubtitleModel);

        handleActions();
    }

    void CrowWindow::updateData(const ProjectData& data)
    {
        ui.episodesWidget->insertProjectEpisodes(data);
    }

    void CrowWindow::clearData()
    {
        ui.episodesWidget->clear();
    }

    void CrowWindow::handleActions()
    {
        // Docks:
        hideCrownsole();
        onShowEpisodeListDock();
        onShowSubtitleViewerDock();

        // Video Player:
        onVideoPositionPositionChanged();
        onVideoPlayerSliderChanged();
        onVolumeSliderChanged();
        onSectionRepeatStopped();
        onPlayButtonClicked();
        onStopButtonClicked();
        onVolumeButtonClicked();

        // Episode List:
        onEpisodeClicked();
        onPreviousButtonClick();
        onNextButtonClick();
        onRepeatButtonClick();

        // Subtitles:
        onSubtitleClicked();

        // Actions:
        populateAudioDeviceAction();
        onExternalSubtitleAction();
    }

    void CrowWindow::resizeEvent(QResizeEvent* event)
    {
        QMainWindow::resizeEvent(event);
        ui.videoPlayer->resizeScene();
    }

    void CrowWindow::hideCrownsole()
    {
        ui.crownsoleDockWidget->hide();
    }

    // Docks:
    void CrowWindow::onShowEpisodeListDock()
    {
        QObject::connect(ui.actionEpisodeList, &QAction::triggered, [&]() {
            ui.episodesDockWidget->show();
        });
    }

    void CrowWindow::onShowSubtitleViewerDock()
    {
        QObject::connect(ui.actionSubtitleViewer, &QAction::triggered, [&]() {
            ui.subtitleDockWidget->show();
        });
    }

    // Events:
    void CrowWindow::onVideoPositionPositionChanged()   
    {
        // Player Controls
        QObject::connect(m_MediaHandler->mediaPlayer(), &QMediaPlayer::positionChanged, [&](long long position) {
            ui.playerControls->blockPlayerSliderSignals(true);

            ui.playerControls->setVideoSliderPosition(position);
            ui.playerControls->setCurrentDurationLabel(position);

            ui.playerControls->blockPlayerSliderSignals(false);
        });

        // External Subtitles:
        QObject::connect(m_MediaHandler->mediaPlayer(), &QMediaPlayer::positionChanged, [&](long long position) {
            if (!ui.videoPlayer->enabledSubtitles())
                return;

            auto subtitle = m_SubtitleHandler->getClosestSubtitle(position, m_SubtitleHandler->currentIndex());
            if (!subtitle.has_value())
                return;

            auto subtitleValue = subtitle.value();
            ui.videoPlayer->setSubtitleText(subtitle.value().text.c_str());           
        });
    }

    void CrowWindow::onVideoPlayerSliderChanged()
    {
        QObject::connect(ui.playerControls->ui.playerSlider, &QSlider::valueChanged, [&](long long position) {
            m_MediaHandler->setMediaPosition(position);
        });
    }

    void CrowWindow::onVolumeSliderChanged()
    {
        QObject::connect(ui.playerControls->ui.volumeSlider, &QSlider::valueChanged, [&](int position) {
            m_MediaHandler->setVolume(position / 100.f);
            ui.playerControls->toggleVolumeButtonFromVolume(position);
        });
    }

    void CrowWindow::onSectionRepeatStopped()
    {
        QObject::connect(m_MediaHandler, &CrowMediaHandler::sectionRepeatStopped, [&]() {
            ui.playerControls->togglePlayButtonIcon(true);
        });
    }

    void CrowWindow::onPlayButtonClicked()
    {
        QObject::connect(ui.playerControls->ui.playVideoBtn, &QPushButton::released, [&]() {
            if (!m_MediaHandler->isMediaSet())
                return;

            bool isMediaPlaying = m_MediaHandler->isMediaPlaying();
            isMediaPlaying ? m_MediaHandler->pause() : m_MediaHandler->play();

            ui.playerControls->togglePlayButtonIcon(isMediaPlaying);
        });
    }

    void CrowWindow::onStopButtonClicked()
    {
        QObject::connect(ui.playerControls->ui.stopVideoBtn, &QPushButton::released, [&]() {
            if (!m_MediaHandler->isMediaSet())
                return;

            m_MediaHandler->stop();
            ui.playerControls->togglePlayButtonIcon(true);
        });
    }

    void CrowWindow::onVideoRepeatClicked()
    {
        QObject::connect(ui.subtitleList->selectionModel(), &QItemSelectionModel::currentChanged, [&](const QModelIndex& current, const QModelIndex& previous) {
            m_MediaHandler->toggleRepeat();
        });
    }

    void CrowWindow::onVolumeButtonClicked()
    {
        QObject::connect(ui.playerControls->ui.volumeBtn, &QPushButton::released, [&]() {
            bool isMuted = m_MediaHandler->isMuted();

            if (!isMuted)
            {
                m_MediaHandler->mute();
                ui.playerControls->toggleVolumeSliderEnabled(false);
                ui.playerControls->toggleVolumeButtonState(PlayerControlsWidget::VolumeState::MUTED);
            }
            else
            {
                m_MediaHandler->unmute();
                ui.playerControls->toggleVolumeSliderEnabled(true);
                ui.playerControls->toggleVolumeButtonFromVolume(ui.playerControls->volume());
            }
        });
    }

    void CrowWindow::onEpisodeClicked()
    {
        QObject::connect(ui.episodesWidget, &EpisodeListWidget::episodeFromListClicked, [&](const std::string& episodeFilepath) {
            if (m_MediaHandler->videoSource() == QUrl::fromLocalFile(episodeFilepath.c_str()))
                return;

            // Clear Subtitles:
            m_SubtitleModel.clear();
            ui.menuSubtitleTrack->clear();

            m_MediaHandler->setMedia(episodeFilepath);
            m_MediaHandler->play();

            long long duration = m_MediaHandler->duration();
            ui.playerControls->setVideoSliderMaximum(duration);
            ui.playerControls->setTotalDurationLabel(duration);

            ui.playerControls->togglePlayButtonIcon(false);

            toggleAudioTrackAction(true);
            toggleSubtitleTrackAction(true);
            toggleExternalSubtitleAction(true);

            populateSubtitleTrackAction();
            populateAudioTrackAction();
        });
    }

    void CrowWindow::onPreviousButtonClick()
    {
        auto* episodesWidget = ui.episodesWidget;

        QObject::connect(ui.playerControls->ui.previousBtn, &QPushButton::released, [&, episodesWidget]() {
            auto episodeIndex = episodesWidget->setPreviousEpisode();
            if (!episodeIndex.has_value())
                return;

            auto source = episodesWidget->getSourceFromIndex(episodeIndex.value());
            if (!source.has_value())
                return;

            m_MediaHandler->setMedia(source.value());
            m_MediaHandler->play();
        });
    }

    void CrowWindow::onNextButtonClick()
    {
        auto episodesWidget = ui.episodesWidget;

        QObject::connect(ui.playerControls->ui.nextBtn, &QPushButton::released, [&, episodesWidget]() {
            auto episodeIndex = episodesWidget->setNextEpisode();
            if (!episodeIndex.has_value())
                return;

            auto source = episodesWidget->getSourceFromIndex(episodeIndex.value());
            if (!source.has_value())
                return;

            m_MediaHandler->setMedia(source.value());
            m_MediaHandler->play();
        });
    }

    void CrowWindow::onRepeatButtonClick()
    {
        QObject::connect(ui.playerControls->ui.repeatBtn, &QPushButton::released, [&]() {
            bool wasEnabled = ui.playerControls->ui.repeatBtn->isEnabled();
            ui.playerControls->toggleRepeatButtonChecked(!wasEnabled);

            if (wasEnabled)
            {

                m_MediaHandler->setRepeat(false);
            }
            else
            {
                m_MediaHandler->setRepeatTimestamp(0, m_MediaHandler->duration());
                m_MediaHandler->setRepeat(true);
            }

            ui.playerControls->togglePlayButtonIcon(false);
            m_MediaHandler->play();
        });
    }

    void CrowWindow::onSubtitleClicked()
    {
        QObject::connect(ui.subtitleList, &QAbstractItemView::clicked, [&](const QModelIndex& current) {
            m_MediaHandler->pause();

            ui.playerControls->toggleRepeatButtonChecked(true);
            ui.playerControls->togglePlayButtonIcon(false);

            auto data = m_SubtitleModel.getDataAtModelIndex(current.row());
            auto start = data.startTimeMilliseconds;
            auto end = data.endTimeMilliseconds;

            m_MediaHandler->mediaPlayer()->blockSignals(true);

            m_MediaHandler->setMediaPosition(start);
            m_MediaHandler->setRepeatTimestamp(start, end);
            m_MediaHandler->setRepeat(true);
            m_MediaHandler->play();
            
            m_MediaHandler->mediaPlayer()->blockSignals(false);
        });
    }

    // Actions:
    void CrowWindow::toggleAudioTrackAction(bool value)
    {
        ui.menuAudioTrack->setEnabled(value);
    }

    void CrowWindow::toggleSubtitleTrackAction(bool value)
    {
        ui.menuSubtitleTrack->setEnabled(value);
    }

    void CrowWindow::toggleExternalSubtitleAction(bool value)
    {
        ui.actionAddExternalTrack->setEnabled(value);
    }

    void CrowWindow::populateAudioTrackAction()
    {
        auto tracks = m_MediaHandler->audioTracks();

        ui.menuAudioTrack->clear();

        for (size_t i = 0; i < tracks.size(); ++i)
        {
            auto title = tracks[i].value(QMediaMetaData::Title);

            QAction* action = new QAction(ui.menuAudioTrack);
            action->setText(title.toString());
            action->setCheckable(true);

            if (m_MediaHandler->activeAudioTrack() == i)
                action->setChecked(true);

            QObject::connect(action, &QAction::triggered, [&, action, i](bool checked) {
                if (!checked)
                    action->setChecked(true);

                for (const auto& audioAction : ui.menuAudioTrack->actions())
                {
                    if (audioAction != action)
                        audioAction->setChecked(false);
                }

                m_MediaHandler->setActiveAudioTrack(i);
            });

            ui.menuAudioTrack->addAction(action);
        }
    }

    void CrowWindow::populateAudioDeviceAction()
    {
        ui.menuAudioDevice->clear();

        for (const auto& outputDevice : QMediaDevices::audioOutputs())
        {
            QAction* action = new QAction(ui.menuAudioDevice);
            action->setText(outputDevice.description());
            action->setCheckable(true);

            if (outputDevice == QMediaDevices::defaultAudioOutput())
                action->setChecked(true);

            QObject::connect(action, &QAction::triggered, [&, action, outputDevice](bool checked) {
                if (!checked)
                    action->setChecked(true);

                for (const auto& audioAction : ui.menuAudioDevice->actions())
                {
                    if (audioAction != action)
                        audioAction->setChecked(false);
                }

                m_MediaHandler->setAudioDevice(outputDevice);
            });

            ui.menuAudioDevice->addAction(action);
        }
    }

    void CrowWindow::populateSubtitleTrackAction()
    {
        auto tracks = m_MediaHandler->subtitleTracks();

        ui.menuSubtitleTrack->clear();

        auto subtitleTrackAction = [&](QAction* action, int index) {
            QObject::connect(action, &QAction::triggered, [&, action, index](bool checked) {
                if (!checked)
                    action->setChecked(true);

                for (const auto& audioAction : ui.menuSubtitleTrack->actions())
                {
                    if (audioAction != action)
                        audioAction->setChecked(false);
                }

                ui.videoPlayer->setSubtitleText("");
                ui.videoPlayer->setEnabledSubtitles(false);
                m_MediaHandler->setActiveSubtitleTrack(index);
            });
        };

        // Default disabled subtitle track:
        {
            QAction* action = new QAction(ui.menuSubtitleTrack);
            action->setText("Disabled");
            action->setCheckable(true);
            action->setChecked(true);

            subtitleTrackAction(action, -1);
            ui.menuSubtitleTrack->addAction(action);
        }

        for (size_t i = 0; i < tracks.size(); ++i)
        {
            auto title = tracks[i].value(QMediaMetaData::Title).toString();
            auto language = tracks[i].value(QMediaMetaData::Language).toString();

            QAction* action = new QAction(ui.menuSubtitleTrack);
            action->setText(title + " - " + language);
            action->setCheckable(true);

            subtitleTrackAction(action, i);

            ui.menuSubtitleTrack->addAction(action);
        }
    }

    void CrowWindow::onExternalSubtitleAction()
    {
        QObject::connect(ui.actionAddExternalTrack, &QAction::triggered, [&]() {
            std::string filepath = openSubtitleTrackDialog();
            if (filepath.empty())
                return;

            auto index = m_SubtitleHandler->load(filepath);
            if (!index.has_value())
                return;

            size_t i = index.value();

            QFileInfo fileInfo(filepath.c_str());

            QAction* action = new QAction(ui.menuAudioTrack);
            action->setText(fileInfo.fileName());
            action->setCheckable(true);
            
            QObject::connect(action, &QAction::triggered, [&, action, i](bool checked) {
                if (!checked)
                    action->setChecked(true);

                for (const auto& audioAction : ui.menuSubtitleTrack->actions())
                {
                    if (audioAction != action)
                        audioAction->setChecked(false);
                }

                if (m_SubtitleHandler->subtitles(i).empty())
                    return;

                m_SubtitleModel.populateData(m_SubtitleHandler->subtitles(i));
                ui.videoPlayer->setEnabledSubtitles(true);
                m_MediaHandler->setActiveSubtitleTrack(-1);
            });

            action->trigger();
            ui.menuSubtitleTrack->addAction(action);
        });
    }
}