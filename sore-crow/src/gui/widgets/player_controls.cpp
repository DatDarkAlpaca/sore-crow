#include "pch.h"
#include "player_controls.h"
#include "utils/string_utils.h"

namespace sore
{
	PlayerControlsWidget::PlayerControlsWidget(QWidget* parent)
		: QWidget(parent)
	{
		ui.setupUi(this);

		setupAudioSlider();

		onPlayerSliderPositionChanged();
	}

	void PlayerControlsWidget::togglePlayButtonIcon(bool togglePlayingIcon)
	{
		if (togglePlayingIcon)
			ui.playVideoBtn->setIcon(QIcon(":/main/icons/play.png"));
		else
			ui.playVideoBtn->setIcon(QIcon(":/main/icons/pause.png"));
	}

	void PlayerControlsWidget::toggleVolumeButtonState(VolumeState state)
	{
		switch(state)
		{
			case VolumeState::MUTED:
				ui.volumeBtn->setIcon(QIcon(":/main/icons/volume_none.png"));
				break;

			case VolumeState::HALF:
				ui.volumeBtn->setIcon(QIcon(":/main/icons/volume_two.png"));
				break;

			case VolumeState::MAX:
				ui.volumeBtn->setIcon(QIcon(":/main/icons/volume_full.png"));
				break;
		}
	}

	void PlayerControlsWidget::toggleVolumeButtonFromVolume(int volume)
	{
		if (volume == 0)
			toggleVolumeButtonState(PlayerControlsWidget::VolumeState::MUTED);

		else if (volume > 0 && volume <= 99)
			toggleVolumeButtonState(PlayerControlsWidget::VolumeState::HALF);

		else
			toggleVolumeButtonState(PlayerControlsWidget::VolumeState::MAX);
	}

	void PlayerControlsWidget::setVideoSliderPosition(long long position)
	{
		ui.playerSlider->setValue(position);
	}

	void PlayerControlsWidget::setVideoSliderMaximum(long long maximum)
	{
		ui.playerSlider->setMaximum(maximum);
	}

	void PlayerControlsWidget::blockPlayerSliderSignals(bool value)
	{
		ui.playerSlider->blockSignals(value);
	}

	void PlayerControlsWidget::toggleVolumeSliderEnabled(bool value)
	{
		ui.volumeSlider->setEnabled(value);
	}

	int PlayerControlsWidget::volume() const
	{
		return ui.volumeSlider->value();
	}

	void PlayerControlsWidget::onPlayerSliderPositionChanged()
	{
		QObject::connect(ui.playerSlider, &QSlider::valueChanged, [&](long long position) {
			setCurrentDurationLabel(position);
		});
	}

	void PlayerControlsWidget::setCurrentDurationLabel(long long duration)
	{
		ui.currentPositionLabel->setText(getDurationString(duration).c_str());
	}

	void PlayerControlsWidget::setTotalDurationLabel(long long duration)
	{
		ui.totalDurationLabel->setText(getDurationString(duration).c_str());
	}

	void PlayerControlsWidget::setupAudioSlider()
	{
		ui.volumeSlider->setMaximum(100);
		ui.volumeSlider->setMinimum(0);
		ui.volumeSlider->setValue(50);
	}
}