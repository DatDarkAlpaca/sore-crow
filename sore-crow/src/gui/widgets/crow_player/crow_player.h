#pragma once
#include "core/mpv/mpv_widget.h"

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

		void addExternalSubtitle(const QString& filepath);
		void setSubtitleVisibility(bool visible);
		void setSubtitleTrack(int64_t id);
		void setSubtitleTrack(const QString& stream);
		QString getSubtitle();

	signals:
		void playingStatusChanged(bool isPlaying);

	public:
		int volumeMax() const;

		long long duration() const;

		bool isPlaying() const;

		bool isPaused() const;

	private:
		void showContextMenu(const QPoint& pos);

	private:
		bool m_IsMuted = false;
		int m_PreviousVolume = 0;
	};
}