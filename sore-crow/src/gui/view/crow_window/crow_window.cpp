#include "pch.h"
#include "crow_window.h"
#include "utils/dialog.h"
#include "core/logger/logger.h"

#include <QGraphicsVideoItem>
#include <QVideoSink>

namespace sore
{
    CrowWindow::CrowWindow(QWidget* parent)
        : QMainWindow(parent)
    {
        ui.setupUi(this);

        m_MediaHandler = new CrowMediaHandler(this);
        m_MediaHandler->setVideoOutput(ui.videoPlayer->videoItem());

        onVideoPositionPositionChanged();
        onVideoPlayerSliderChanged();


        onEpisodeClicked();
        onPreviousButtonClick();
        onNextButtonClick();

        // Docks:
        onShowEpisodeListDock();
        onShowSubtitleViewerDock();

        // Actions:
        populateAudioDeviceAction();
    }

    void CrowWindow::updateData(const ProjectData& data)
    {
        ui.episodesWidget->insertProjectEpisodes(data);
    }

    void CrowWindow::clearData()
    {
        ui.episodesWidget->clear();
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
        QObject::connect(m_MediaHandler->mediaPlayer(), &QMediaPlayer::positionChanged, [&](long long position) {
            ui.playerControls->blockPlayerSliderSignals(true);

            ui.playerControls->setVideoSliderPosition(position);
            ui.playerControls->setCurrentDurationLabel(position);

            ui.playerControls->blockPlayerSliderSignals(false);
        });
    }

    void CrowWindow::onVideoPlayerSliderChanged()
    {
        QObject::connect(ui.playerControls->ui.playerSlider, &QSlider::valueChanged, [&](long long position) {
            m_MediaHandler->setMediaPosition(position);
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

                // TODO: use the CrowVideo widget to set the subtitle.
                // TODO: create a mediator between crow video and media handler for subtitle position changed
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
}