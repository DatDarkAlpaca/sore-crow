#pragma once
#include "core/mpv/mpv_widget.h"
#include "core/stylesheet/subtitle_font_styles.h"

namespace sore
{
	class CrowPlayer : public MPVWidget
	{
		Q_OBJECT

	public:
		CrowPlayer(QWidget* parent = nullptr);
	
	public slots:
		void openMedia(const QString& filepath, bool appendFile = false, const QStringList& options = QStringList());

		void seek(double positionMs, SeekFlag flag = SeekFlag::SEEK_ABSOLUTE);
		void seekAbsolute(double positionMs);

		void play();
		void pause();
		void stop();
		void togglePlay();
		void toggleRepeat();
		void setRepeat(bool repeating);
		void setRepeatInterval(double start, double end);

		void playlistPlay(int index);
		void playlistNext();
		void playlistPrev();
		void clearPlaylist();

		void setVolumeMax(int volumeMax);
		void setVolume(int volume);
		void mute();
		void unmute();
		void toggleMute();

		void setAudioTrack(int64_t id);
		void setAudioDevice(const QString& name);

		// Subtitles:
		void addExternalSubtitle(const QString& filepath);
		void setSubtitleVisibility(bool visible);
		void setSecondarySubtitleVisibility(bool visible);

		void setSubtitleTrack(int64_t id);
		void setSecondarySubtitleTrack(int64_t id);

		void setSubtitleTrack(const QString& stream);
		void setSecondarySubtitleTrack(const QString& stream);
		QString getSubtitle() const;
		QString getSecondarySubtitle() const;

		void overrideSubtitleStyles(bool shouldOverride);
		void setSubtitleStyle(const SubtitleFontStyles& style);

	signals:
		void playingStatusChanged(bool isPlaying);

	public:
		int volumeMax() const;

		long long duration() const;

		bool isPlaying() const;

		bool isPaused() const;

	private:
		void showContextMenu(const QPoint& pos);

		void handleRepeatOnPositionChange(double position);

	private:
		bool m_IsMuted = false, m_IsLooping = false;
		double m_StartLoop = 0, m_EndLoop = 0;
		int m_PreviousVolume = 0;
	};
}