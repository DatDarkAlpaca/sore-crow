#pragma once
#include <QtWidgets/QWidget>
#include "ui_player_controls.h"

namespace sore
{
	class PlayerControlsWidget : public QWidget
	{
		Q_OBJECT

	public:
		PlayerControlsWidget(QWidget* parent = nullptr);

	// Control Buttons:
	public:
		void togglePlayButtonIcon(bool togglePlayingIcon);

	// Video Slider:
	public:
		void setVideoSliderPosition(long long position);

		void setVideoSliderMaximum(long long videoDuration);

		void blockPlayerSliderSignals(bool value);

		void onPlayerSliderPositionChanged();

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