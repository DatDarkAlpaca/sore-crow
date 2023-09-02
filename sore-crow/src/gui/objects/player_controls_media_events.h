#pragma once
#include "gui/objects/crow_video.h"
#include "gui/widgets/player_controls.h"

namespace sore
{
	class PlayerControlsMediaEvents : public QObject
	{
		Q_OBJECT

	public:
		PlayerControlsMediaEvents(QObject* parent, CrowMediaHandler& mediaHandler, PlayerControlsWidget& controlsWidget)
			: QObject(parent)
			, m_MediaHandler(mediaHandler)
			, m_ControlsWidget(controlsWidget)
		{
			onVideoPositionChanged();

			onVolumeSliderPositionChange();
			onVolumeButtonClicked();

			// Player Control buttons:
			onPlayButtonClicked();
			onStopButtonClicked();
		}

	private:
		void onVideoPositionChanged()
		{
			// On MediaPlayer running:
			QObject::connect(m_MediaHandler.mediaPlayer(), &QMediaPlayer::positionChanged, [&](long long position) {
				m_ControlsWidget.blockPlayerSliderSignals(true);

				m_ControlsWidget.setVideoSliderPosition(position);
				m_ControlsWidget.setCurrentDurationLabel(position);

				m_ControlsWidget.blockPlayerSliderSignals(false);
			});

			// On PlayerControls VideoSlider changed:
			QObject::connect(m_ControlsWidget.ui.playerSlider, &QSlider::valueChanged, [&](long long position) {
				m_MediaHandler.setMediaPosition(position);
			});
		}

		void onVolumeSliderPositionChange()
		{
			QObject::connect(m_ControlsWidget.ui.volumeSlider, &QSlider::valueChanged, [&](int position) {
				m_MediaHandler.setVolume(position / 100.f);
				m_ControlsWidget.toggleVolumeButtonFromVolume(position);
			});
		}

		void onVolumeButtonClicked()
		{
			QObject::connect(m_ControlsWidget.ui.volumeBtn, &QPushButton::released, [&]() {
				bool isMuted = m_MediaHandler.isMuted();

				if (!isMuted)
				{
					m_MediaHandler.mute();
					m_ControlsWidget.toggleVolumeSliderEnabled(false);
					m_ControlsWidget.toggleVolumeButtonState(PlayerControlsWidget::VolumeState::MUTED);
				}

				else
				{
					m_MediaHandler.unmute();
					m_ControlsWidget.toggleVolumeSliderEnabled(true);
					m_ControlsWidget.toggleVolumeButtonFromVolume(m_ControlsWidget.volume());
				}
			});
		}

	private:
		void onPlayButtonClicked()
		{
			// On PlayerControls PlayButton pressed:
			QObject::connect(m_ControlsWidget.ui.playVideoBtn, &QPushButton::released, [&]() {
				if (!m_MediaHandler.isMediaSet())
					return;

				bool isMediaPlaying = m_MediaHandler.isMediaPlaying();
				isMediaPlaying ? m_MediaHandler.pause() : m_MediaHandler.play();

				m_ControlsWidget.togglePlayButtonIcon(isMediaPlaying);
			});
		}

		void onStopButtonClicked()
		{
			QObject::connect(m_ControlsWidget.ui.stopVideoBtn, &QPushButton::released, [&]() {
				if (!m_MediaHandler.isMediaSet())
					return;

				m_MediaHandler.stop();
				m_ControlsWidget.togglePlayButtonIcon(true);
			});
		}		

	private:
		CrowMediaHandler& m_MediaHandler;
		PlayerControlsWidget& m_ControlsWidget;
	};
}