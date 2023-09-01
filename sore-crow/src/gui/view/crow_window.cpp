#include "pch.h"
#include "crow_window.h"
#include "utils/string_utils.h"

namespace sore
{
    CrowWindow::CrowWindow(QWidget* parent)
        : QMainWindow(parent)
    {
        ui.setupUi(this);

        setupPlayer();

        setupAudioSlider();
        m_AudioOutput->setVolume(50.0f);

        setupIcons();

        handleVideoPositionChange();
        handleVolumePositionChange();

        handlePlayButtonClicked();
        handleStopButtonClick();
        handlePreviousButtonClick();
        handleNextButtonClick();

        populateAudioDevice();
    }

    void CrowWindow::updateData(const ProjectData& data)
    {
        insertProjectEpisodes(data);
    }

    void CrowWindow::clearData()
    {
        ui.episodesList->clear();
    }

    void CrowWindow::insertProjectEpisodes(const ProjectData& data)
    {
        using namespace std::filesystem;

        path root(data.rootFolder);
        path episodeFolder(data.episodeFolderName);

        for (const auto& episode : data.sourceMetadata.episodes)
        {
            path episodePath = root / episodeFolder / path(episode.filename);

            EpisodeWidget* episodeWidget = new EpisodeWidget(this, episodePath.string());
            episodeWidget->updateData(episodePath.string());
            handleEpisodeClicked(episodeWidget, episodePath.string());

            addEpisodeToDock(episodeWidget);
        }
    }

    void CrowWindow::handleEpisodeClicked(EpisodeWidget* episodeWidget, const std::string& episodeFilepath)
    {
        QObject::connect(episodeWidget, &EpisodeWidget::episodeClicked, [this, episodeFilepath]() {
            setVideo(episodeFilepath);
            long long duration = m_MediaPlayer->duration();
            
            setVideoSliderMaximum(duration);
            setTotalDurationLabel(duration);
            togglePlayButtonIcon(false);

            toggleAudioTrackAction(true);
            populateAudioTrackAction();

            m_MediaPlayer->play();
        });
    }

    void CrowWindow::addEpisodeToDock(EpisodeWidget* episodeWidget)
    {
        QListWidgetItem* item = new QListWidgetItem(ui.episodesList);

        item->setSizeHint(episodeWidget->sizeHint());

        ui.episodesList->addItem(item);
        ui.episodesList->setItemWidget(item, episodeWidget);
    }

    void CrowWindow::setVideo(const std::string& videoFilepath)
    {
        m_MediaPlayer->setSource(QUrl::fromLocalFile(videoFilepath.c_str()));
    }

    void CrowWindow::setVideoSliderMaximum(long long videoDuration)
    {
        if (!hasVideoSource())
        {
            qDebug() << "No source detected. Cant set slider";
            return;
        }

        ui.playerSlider->setMaximum(videoDuration);
    }

    void CrowWindow::handleVideoPositionChange()
    {
        QObject::connect(m_MediaPlayer, &QMediaPlayer::positionChanged, [&](long long position) {
            ui.playerSlider->blockSignals(true);
            ui.playerSlider->setValue(position);
            setCurrentDurationLabel(position);
            ui.playerSlider->blockSignals(false);
        });

        QObject::connect(ui.playerSlider, &QSlider::valueChanged, [&](long long position) {
            m_MediaPlayer->setPosition(position);
            setCurrentDurationLabel(position);
        });
    }

    inline bool CrowWindow::hasVideoSource()
    {
        return !m_MediaPlayer->source().isEmpty();
    }

    void CrowWindow::toggleAudioTrackAction(bool value)
    {
        ui.menuAudioTrack->setEnabled(value);
    }

    void CrowWindow::populateAudioTrackAction()
    {
        auto tracks = m_MediaPlayer->audioTracks();

        ui.menuAudioTrack->clear();

        for (size_t i = 0; i < tracks.size(); ++i)
        {
            auto title = tracks[i].value(QMediaMetaData::Title);

            QAction* action = new QAction(ui.menuAudioTrack);
            action->setText(title.toString());
            
            ui.menuAudioTrack->addAction(action);

            QObject::connect(action, &QAction::triggered, [&, i]() {
                m_MediaPlayer->setActiveAudioTrack(i);
            });
        }
    }

    void CrowWindow::populateAudioDevice()
    {
        ui.menuAudioDevice->clear();

        for (const auto& outputDevice : QMediaDevices::audioOutputs())
        {
            QAction* action = new QAction(ui.menuAudioDevice);
            action->setText(outputDevice.description());

            QObject::connect(action, &QAction::triggered, [&, outputDevice]() {
                m_AudioOutput->setDevice(outputDevice);
            });

            ui.menuAudioDevice->addAction(action);
        }
    }

    void CrowWindow::handleVolumePositionChange()
    {
        QObject::connect(ui.volumeSlider, &QSlider::valueChanged, [&](int position) {
            m_AudioOutput->setVolume(position / 100.f);
        });
    }

    void CrowWindow::setCurrentDurationLabel(long long duration)
    {
        ui.currentPositionLabel->setText(getDurationString(duration).c_str());
    }

    void CrowWindow::setTotalDurationLabel(long long duration)
    {
        ui.totalDurationLabel->setText(getDurationString(duration).c_str());
    }

    void CrowWindow::togglePlayButtonIcon(bool togglePlayingIcon)
    {
        if(togglePlayingIcon)
            ui.playVideoBtn->setIcon(QIcon("C:/Users/paulo/Projects/sore-crow/sore-crow/resources/icons/play.png"));
        else
            ui.playVideoBtn->setIcon(QIcon("C:/Users/paulo/Projects/sore-crow/sore-crow/resources/icons/pause.png"));
    }

    void CrowWindow::handlePlayButtonClicked()
    {
        QObject::connect(ui.playVideoBtn, &QPushButton::released, [&]() {
            if (!hasVideoSource())
                return;

            bool isPlaying = m_MediaPlayer->isPlaying();
            isPlaying ? m_MediaPlayer->pause() : m_MediaPlayer->play();

            togglePlayButtonIcon(isPlaying);
        });
    }

    void CrowWindow::handleStopButtonClick()
    {
        QObject::connect(ui.stopVideoBtn, &QPushButton::released, [&]() {
            if (!hasVideoSource())
                return;

            m_MediaPlayer->stop();
            togglePlayButtonIcon(true);
        });
    }

    void CrowWindow::handlePreviousButtonClick()
    {
        QObject::connect(ui.previousBtn, &QPushButton::released, [&]() {
            QListWidgetItem* item = ui.episodesList->currentItem();
            if (!item)
                return;

            int currentIndex = ui.episodesList->row(item);
            if (currentIndex == 0)
                return;

            int nextIndex = currentIndex - 1;
            QListWidgetItem* next = ui.episodesList->item(nextIndex);
            if (!next)
                return;

            ui.episodesList->setCurrentItem(next);

            EpisodeWidget* episodeWidget = static_cast<EpisodeWidget*>(ui.episodesList->itemWidget(next));

            setVideo(episodeWidget->getEpisodeFilepath());
            m_MediaPlayer->play();
        });
    }

    void CrowWindow::handleNextButtonClick()
    {
        QObject::connect(ui.nextBtn, &QPushButton::released, [&]() {
            QListWidgetItem* item = ui.episodesList->currentItem();
            if (!item)
                return;

            int currentIndex = ui.episodesList->row(item);
            if (currentIndex == ui.episodesList->count() - 1)
                return;

            int nextIndex = currentIndex + 1;
            QListWidgetItem* next = ui.episodesList->item(nextIndex);
            if (!next)
                return;

            ui.episodesList->setCurrentItem(next);

            EpisodeWidget* episodeWidget = static_cast<EpisodeWidget*>(ui.episodesList->itemWidget(next));

            setVideo(episodeWidget->getEpisodeFilepath());
            m_MediaPlayer->play();
        });
    }

    void CrowWindow::setupPlayer()
    {
        m_MediaPlayer = new QMediaPlayer(this);
        m_AudioOutput = new QAudioOutput(this);
        m_MediaPlayer->setVideoOutput(ui.videoPlayer);
        m_MediaPlayer->setAudioOutput(m_AudioOutput);
    }

    void CrowWindow::setupAudioSlider()
    {
        ui.volumeSlider->setMaximum(100);
        ui.volumeSlider->setMinimum(0);
        ui.volumeSlider->setValue(50);
    }

    void CrowWindow::setupIcons()
    {
        ui.playVideoBtn->setText("");
        ui.stopVideoBtn->setText("");
        ui.previousBtn->setText("");
        ui.nextBtn->setText("");

        // TODO: set relative paths when the file system is ready
        ui.playVideoBtn->setIcon(QIcon("C:/Users/paulo/Projects/sore-crow/sore-crow/resources/icons/play.png"));
        ui.stopVideoBtn->setIcon(QIcon("C:/Users/paulo/Projects/sore-crow/sore-crow/resources/icons/stop.png"));
        ui.previousBtn->setIcon(QIcon("C:/Users/paulo/Projects/sore-crow/sore-crow/resources/icons/previous.png"));
        ui.nextBtn->setIcon(QIcon("C:/Users/paulo/Projects/sore-crow/sore-crow/resources/icons/next.png"));
    }
}