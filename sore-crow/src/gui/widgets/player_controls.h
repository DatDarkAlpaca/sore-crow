#pragma once
#include <QtWidgets/QWidget>
#include "ui_player_controls.h"

namespace sore
{
	class PlayerControlsWidget : public QWidget
	{
		Q_OBJECT

	public:
		enum class VolumeState : uint32_t { MUTED = 0, HALF, MAX };

	public:
		PlayerControlsWidget(QWidget* parent = nullptr);

	// Control Buttons:
	public:
		void togglePlayButtonIcon(bool togglePlayingIcon);

		void toggleVolumeButtonState(VolumeState state);

		void toggleVolumeButtonFromVolume(int volume);

	// Video Slider:
	public:
		void setVideoSliderPosition(long long position);

		void setVideoSliderMaximum(long long videoDuration);

		void blockPlayerSliderSignals(bool value);

		void onPlayerSliderPositionChanged();

	// Volume Slider:
		void toggleVolumeSliderEnabled(bool value);

		int volume() const;

	// Duration labels:
	public:
		void setCurrentDurationLabel(long long duration);

		void setTotalDurationLabel(long long duration);

	private:
		void setupAudioSlider();

	public:
		Ui::PlayerControls ui;
	};
}