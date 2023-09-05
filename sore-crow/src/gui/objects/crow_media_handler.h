#pragma once
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMediaMetaData>

#include "gui/widgets/player_controls/player_controls.h"

namespace sore
{
	class CrowMediaHandler : public QObject
	{
		Q_OBJECT

	public:
		CrowMediaHandler(QObject* parent = nullptr)
			: QObject(parent)
		{
			m_MediaPlayer = new QMediaPlayer(parent);
			m_AudioOutput = new QAudioOutput(parent);

			m_MediaPlayer->setAudioOutput(m_AudioOutput);
			m_AudioOutput->setVolume(50.f);
		}

	public:
		void setMedia(const std::string& mediaFilepath)
		{
			m_MediaPlayer->setSource(QUrl::fromLocalFile(mediaFilepath.c_str()));
		}

		void setMediaPosition(long long position)
		{
			m_MediaPlayer->setPosition(position);
		}

		bool isMediaSet() const
		{
			return !m_MediaPlayer->source().isEmpty();
		}

		bool isMediaPlaying() const
		{
			return m_MediaPlayer->isPlaying();
		}

		bool isMuted() const
		{
			return m_AudioOutput->isMuted();
		}

		long long duration() const
		{
			return m_MediaPlayer->duration();
		}

		void setVolume(float volume)
		{
			m_AudioOutput->setVolume(volume);
		}

	public:
		void setVideoOutput(QObject* outputObject)
		{
			m_MediaPlayer->setVideoOutput(outputObject);
		}

		void setAudioDevice(const QAudioDevice& device)
		{
			m_AudioOutput->setDevice(device);
		}

		QUrl videoSource() const
		{
			return m_MediaPlayer->source();
		}

	public:
		int activeAudioTrack() const
		{
			return m_MediaPlayer->activeAudioTrack();
		}

		void setActiveAudioTrack(int index)
		{
			m_MediaPlayer->setActiveAudioTrack(index);
		}

		QList<QMediaMetaData> audioTracks() const
		{
			return m_MediaPlayer->audioTracks();
		}

	public:
		int activeSubtitleTrack() const
		{
			return m_MediaPlayer->activeSubtitleTrack();
		}

		void setActiveSubtitleTrack(int index)
		{
			m_MediaPlayer->setActiveSubtitleTrack(index);
		}

		QList<QMediaMetaData> subtitleTracks() const
		{
			return m_MediaPlayer->subtitleTracks();
		}

	public:
		inline void play() { m_MediaPlayer->play(); }

		inline void stop() { m_MediaPlayer->stop(); }

		inline void pause() { m_MediaPlayer->pause(); }

		inline void mute() { m_AudioOutput->setMuted(true); }

		inline void unmute() { m_AudioOutput->setMuted(false); }

	public:
		QMediaPlayer* mediaPlayer() const
		{
			return m_MediaPlayer;
		}

		QAudioOutput* audioOutput() const
		{
			return m_AudioOutput;
		}

	private:
		QMediaPlayer* m_MediaPlayer = nullptr;
		QAudioOutput* m_AudioOutput = nullptr;
	};
}