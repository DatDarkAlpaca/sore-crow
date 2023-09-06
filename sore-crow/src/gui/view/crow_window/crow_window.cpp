#include "pch.h"
#include "crow_window.h"
#include "utils/dialog.h"
#include "core/logger/logger.h"
#include "core/parser/subtitle_parser.h"

#include <QGraphicsVideoItem>
#include <QVideoSink>

namespace sore
{
    CrowWindow::CrowWindow(QWidget* parent)
        : QMainWindow(parent)
    {
        ui.setupUi(this);

        // Crownsole:
        CrownsoleLogger::setConsoleWidget(ui.crownsole);

        // Video Controller:
        m_MediaHandler = new CrowMediaHandler(this);
        m_MediaHandler->setVideoOutput(ui.videoPlayer->videoItem());

        // Player Control Events:
        m_PlayerControlsEvents = new PlayerControlsMediaEvents(this, *m_MediaHandler, *ui.playerControls);

        // Managed Events:
        onEpisodeClicked();
        onPreviousButtonClick();
        onNextButtonClick();

        // Subtitle Handler:
        m_SubtitleHandler = new SubtitleHandler(*ui.videoPlayer);
        onExternalSubtitleAction();

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
    void CrowWindow::onEpisodeClicked()
    {
        QObject::connect(ui.episodesWidget, &EpisodeListWidget::episodeFromListClicked, [&](const std::string& episodeFilepath) {
            if (m_MediaHandler->videoSource() == QUrl::fromLocalFile(episodeFilepath.c_str()))
                return;

            m_MediaHandler->setMedia(episodeFilepath);
            long long duration = m_MediaHandler->duration();

            ui.playerControls->setVideoSliderMaximum(duration);
            ui.playerControls->setTotalDurationLabel(duration);

            ui.playerControls->togglePlayButtonIcon(false);

            toggleAudioTrackAction(true);
            populateAudioTrackAction();

            toggleSubtitleTrackAction(true);
            populateSubtitleTrackAction();

            toggleExternalSubtitleAction(true);

            m_MediaHandler->play();
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

    void CrowWindow::onExternalSubtitleAction()
    {
        QObject::connect(ui.actionAddExternalTrack, &QAction::triggered, [&](bool checked) {
            std::string filepath = openSubtitleTrackDialog();
            if (filepath.empty())
                return;

            SubtitleParser parser;
            std::vector<SubtitleEntry> entries = parser.parseSubtitleFile(filepath);
            m_SubtitleHandler->setSubtitleFiles(entries);
        });
    }
}