#pragma once
#include "ui_player_controls.h"

namespace sore
{
	class PlayerControls : public QWidget
	{
		Q_OBJECT

	public:
		PlayerControls(QWidget* parent = nullptr);

	public:
		void enableControls();

		void disableControls();

	public:
		void setRepeatChecked(bool checked);

		void setPlaying(bool isPlaying);

		void setVolume(long long volume);

		void setVolumeMaximum(int volumeMax);

		void trySetPosition(long long position);

		void setPosition(long long position);

		void setDuration(long long duration);

	public:
		void blockPlayerSliderSignals(bool block);

	private:
		void updatePlayingIcon();

		void updateVolumeIcon();

	private:
		void handleEvents();

	private:
		void handlePreviousBtn();

		void handlePlayBtn();

		void handleStopBtn();

		void handleNextBtn();

		void handleRepeatBtn();

		void handleVolumeBtn();

	private:
		void handleVolumeSlider(int volume);

		void handlePositionSlider(long long position);

	signals:
		void previousBtnClicked();

		void playBtnClicked();

		void stopBtnClicked();

		void nextBtnClicked();

		void repeatBtnClicked();

		void volumeBtnClicked(int volume);

	signals:
		void volumeChanged(int volume);
	
		void positionChanged(long long position);

	public:
		inline int currentVolume() const { return ui.volumeSlider->value(); }

		inline int maximumVolume() const { return ui.volumeSlider->maximum(); }

		inline bool isRepeatChecked() const { return ui.repeatBtn->isChecked(); }

	private:
		bool m_IsVideoPlaying = false;
		bool m_IsMuted = false;
		int m_PreviousVolume = 0;
		Ui::PlayerControls ui;

	private:
		static constexpr const char* PlayingIcon = ":/Icons/icons/miftakhul/play.png";
		static constexpr const char* PausedIcon = ":/Icons/icons/miftakhul/pause.png";

		static constexpr const char* MaxVolumeIcon = ":/Icons/icons/miftakhul/volume_full.png";
		static constexpr const char* NormalVolumeIcon = ":/Icons/icons/miftakhul/volume_two.png";
		static constexpr const char* MutedVolumeIcon = ":/Icons/icons/miftakhul/volume_none.png";
	};
}