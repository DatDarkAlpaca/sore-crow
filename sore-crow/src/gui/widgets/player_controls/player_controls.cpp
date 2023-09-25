#include "pch.h"
#include "player_controls.h"
#include "utils/string_utils.h"

namespace sore
{
	PlayerControls::PlayerControls(QWidget* parent)
		: QWidget(parent)
	{
		ui.setupUi(this);
		handleEvents();
	}

	void PlayerControls::enableControls()
	{
		ui.playerSlider->setEnabled(true);
		ui.previousBtn->setEnabled(true);
		ui.playVideoBtn->setEnabled(true);
		ui.stopVideoBtn->setEnabled(true);
		ui.nextBtn->setEnabled(true);
		ui.repeatBtn->setEnabled(true);
		ui.volumeBtn->setEnabled(true);

		ui.volumeSlider->setEnabled(true);
		ui.playerSlider->setEnabled(true);
	}

	void PlayerControls::disableControls()
	{
		setPlaying(false);

		ui.playerSlider->setEnabled(false);
		ui.previousBtn->setEnabled(false);
		ui.playVideoBtn->setEnabled(false);
		ui.stopVideoBtn->setEnabled(false);
		ui.nextBtn->setEnabled(false);
		ui.repeatBtn->setEnabled(false);
		ui.volumeBtn->setEnabled(false);

		ui.volumeSlider->setEnabled(false);
		ui.playerSlider->setEnabled(false);
	}

	void PlayerControls::setRepeatChecked(bool checked)
	{
		ui.repeatBtn->setChecked(checked);
	}

	void PlayerControls::setPlaying(bool isPlaying)
	{
		m_IsVideoPlaying = isPlaying;
		updatePlayingIcon();
	}

	void PlayerControls::setVolume(long long volume)
	{
		m_PreviousVolume = currentVolume();
		ui.volumeSlider->setValue(volume);
		updateVolumeIcon();
	}

	void PlayerControls::setVolumeMaximum(int volumeMax)
	{
		ui.volumeSlider->setMaximum(volumeMax);
	}

	void PlayerControls::trySetPosition(long long position)
	{
		if (ui.playerSlider->isBeingDragged())
			return;

		setPosition(position);
	}

	void PlayerControls::setPosition(long long position)
	{
		QString positionString = getDurationString(position);
		ui.currentPositionLabel->setText(positionString);
		ui.playerSlider->setValue(position);
	}

	void PlayerControls::setDuration(long long duration)
	{
		QString durationString = getDurationString(duration);
		ui.totalDurationLabel->setText(durationString);
		ui.playerSlider->setMaximum((int)duration);
	}

	void PlayerControls::blockPlayerSliderSignals(bool block)
	{
		ui.playerSlider->blockSignals(block);
	}

	void PlayerControls::updatePlayingIcon()
	{
		if (m_IsVideoPlaying)
			ui.playVideoBtn->setIcon(QIcon(PausedIcon));
		else
			ui.playVideoBtn->setIcon(QIcon(PlayingIcon));
	}

	void PlayerControls::updateVolumeIcon()
	{
		int volume = currentVolume();

		// Max:
		if (volume >= maximumVolume())
			ui.volumeBtn->setIcon(QIcon(MaxVolumeIcon));

		// Normal:
		else if (volume > 0 && volume < maximumVolume())
			ui.volumeBtn->setIcon(QIcon(NormalVolumeIcon));

		// Muted:
		else
			ui.volumeBtn->setIcon(QIcon(MutedVolumeIcon));
	}

	void PlayerControls::handleEvents()
	{
		// Buttons:
		connect(ui.previousBtn, &QPushButton::released, this, &PlayerControls::handlePreviousBtn);
		connect(ui.playVideoBtn, &QPushButton::released, this, &PlayerControls::handlePlayBtn);
		connect(ui.stopVideoBtn, &QPushButton::released, this, &PlayerControls::handleStopBtn);
		connect(ui.nextBtn, &QPushButton::released, this, &PlayerControls::handleNextBtn);
		connect(ui.repeatBtn, &QPushButton::released, this, &PlayerControls::handleRepeatBtn);
		connect(ui.volumeBtn, &QPushButton::released, this, &PlayerControls::handleVolumeBtn);

		// Slider:
		connect(ui.volumeSlider, &QSlider::sliderMoved, this, &PlayerControls::handleVolumeSlider);
		connect(ui.playerSlider, &QSlider::sliderMoved, this, &PlayerControls::handlePositionSlider);
	}

	void PlayerControls::handlePreviousBtn()
	{
		emit previousBtnClicked();
	}

	void PlayerControls::handlePlayBtn()
	{
		setPlaying(!m_IsVideoPlaying);
		emit playBtnClicked();
	}

	void PlayerControls::handleStopBtn()
	{
		setPlaying(false);
		emit stopBtnClicked();
	}

	void PlayerControls::handleNextBtn()
	{
		emit nextBtnClicked();
	}

	void PlayerControls::handleRepeatBtn()
	{
		emit repeatBtnClicked();
	}

	void PlayerControls::handleVolumeBtn()
	{
		m_IsMuted = !m_IsMuted;

		ui.volumeSlider->setEnabled(!m_IsMuted);

		int newVolume = m_IsMuted ? 0 : 5;
		setVolume(newVolume);

		emit volumeBtnClicked(newVolume);
	}

	void PlayerControls::handleVolumeSlider(int volume)
	{
		setVolume(volume);
		emit volumeChanged(currentVolume());
	}

	void PlayerControls::handlePositionSlider(long long position)
	{
		setPosition(position);
		emit positionChanged(ui.playerSlider->value());
	}
}