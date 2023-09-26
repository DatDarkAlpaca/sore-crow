#include "pch.h"
#include <QClipboard>
#include "crow_player.h"
#include "utils/message_box_utils.h"

namespace sore
{
	CrowPlayer::CrowPlayer(QWidget* parent)
		: MPVWidget(parent)
	{
		setContextMenuPolicy(Qt::CustomContextMenu);
		connect(this, &CrowPlayer::customContextMenuRequested, this, &CrowPlayer::showContextMenu);
	
		connect(this, &CrowPlayer::positionChanged, this, &CrowPlayer::handleRepeatOnPositionChange);
	}

	void CrowPlayer::openMedia(const QString& filepath, bool appendFile, const QStringList& options)
	{
		if (filepath.isEmpty())
			return;

		const char* shouldAppend = appendFile ? "append" : "replace";
		executeCommand(mpvHandle(), QStringList() << "loadfile" << filepath << shouldAppend << options);
	}

	void CrowPlayer::seek(double positionMs, SeekFlag flag)
	{
		executeCommandAsync(mpvHandle(), QVariantList() << "seek" << positionMs / 1000 << seekFlagString(flag));
	}

	void CrowPlayer::seekAbsolute(double positionMs)
	{
		seek(positionMs);
	}

	void CrowPlayer::play()
	{
		mpv::setProperty(mpvHandle(), "pause", false);
		emit playingStatusChanged(true);
	}

	void CrowPlayer::pause()
	{
		mpv::setProperty(mpvHandle(), "pause", true);
		emit playingStatusChanged(false);
	}

	void CrowPlayer::stop()
	{
		pause();
		seek(0, SeekFlag::SEEK_ABSOLUTE_PERCENT);
		emit playingStatusChanged(false);
	}

	void CrowPlayer::togglePlay()
	{
		isPaused() ? play() : pause();
	}

	void CrowPlayer::toggleRepeat()
	{
		m_IsLooping = !m_IsLooping;
	}

	void CrowPlayer::setRepeat(bool repeating)
	{
		m_IsLooping = repeating;
	}

	void CrowPlayer::setRepeatInterval(double start, double end)
	{
		m_StartLoop = start;
		m_EndLoop = end;
	}

	void CrowPlayer::playlistPlay(int index)
	{
		auto playlistSize = mpv::getProperty(mpvHandle(), "playlist/count").toInt();
		executeCommand(mpvHandle(), QVariantList() << "playlist-play-index" << index);

		play();

		emit playingStatusChanged(true);
	}

	void CrowPlayer::playlistNext()
	{
		executeCommand(mpvHandle(), "playlist-next");
	}

	void CrowPlayer::playlistPrev()
	{
		executeCommand(mpvHandle(), "playlist-prev");
	}

	void CrowPlayer::clearPlaylist()
	{
		auto playlistSize = mpv::getProperty(mpvHandle(), "playlist/count").toInt();
		for (size_t i = 0; i < playlistSize; ++i)
			executeCommand(mpvHandle(), QVariantList() << "playlist-remove" << i);

		playlistSize = mpv::getProperty(mpvHandle(), "playlist/count").toInt();
	}

	void CrowPlayer::setVolumeMax(int volumeMax)
	{
		mpv::setPropertyAsync(mpvHandle(), "volume-max", volumeMax);
	}

	void CrowPlayer::setVolume(int volume)
	{
		mpv::setPropertyAsync(mpvHandle(), "volume", volume);
	}

	void CrowPlayer::mute()
	{
		m_PreviousVolume = mpv::getProperty(mpvHandle(), "volume").toInt();
		setVolume(0);
		m_IsMuted = true;
	}

	void CrowPlayer::unmute()
	{
		setVolume(m_PreviousVolume);
		m_IsMuted = false;
	}

	void CrowPlayer::toggleMute()
	{
		(m_IsMuted) ? unmute() : mute();
	}

	void CrowPlayer::setAudioTrack(int64_t id)
	{
		mpv::setPropertyAsync(mpvHandle(), "aid", id);
	}

	void CrowPlayer::setAudioDevice(const QString& name)
	{
		mpv::setPropertyAsync(mpvHandle(), "audio-device", name);
	}

	void CrowPlayer::addExternalSubtitle(const QString& filepath)
	{
		executeCommandAsync(mpvHandle(), QStringList() << "sub-add" << filepath << "cached");
	}

	void CrowPlayer::setSubtitleVisibility(bool visible)
	{
		mpv::setPropertyAsync(mpvHandle(), "sub-visibility", visible);
	}

	void CrowPlayer::setSecondarySubtitleVisibility(bool visible)
	{
		mpv::setPropertyAsync(mpvHandle(), "secondary-sub-visibility", visible);
	}

	void CrowPlayer::setSubtitleTrack(int64_t id)
	{
		setSubtitleVisibility(true);
		mpv::setPropertyAsync(mpvHandle(), "sid", id);
	}

	void CrowPlayer::setSecondarySubtitleTrack(int64_t id)
	{
		setSecondarySubtitleVisibility(true);
		mpv::setPropertyAsync(mpvHandle(), "secondary-sid", id);
	}

	void CrowPlayer::setSubtitleTrack(const QString& stream)
	{
		mpv::setPropertyAsync(mpvHandle(), "sid", stream);
	}

	void CrowPlayer::setSecondarySubtitleTrack(const QString& stream)
	{
		mpv::setPropertyAsync(mpvHandle(), "secondary-sid", stream);
	}

	QString CrowPlayer::getSubtitle() const
	{
		return mpv::getProperty(mpvHandle(), "sub-text").toString();
	}

	QString CrowPlayer::getSecondarySubtitle() const
	{
		return mpv::getProperty(mpvHandle(), "seconadary-sub-text").toString();
	}

	void CrowPlayer::overrideSubtitleStyles(bool shouldOverride)
	{
		mpv::setPropertyAsync(mpvHandle(), "sub-ass-override", shouldOverride ? "force" : "no");
	}

	void CrowPlayer::setSubtitleStyle(const SubtitleFontStyles& style)
	{
		mpv::setPropertyAsync(mpvHandle(), "sub-font", style.family);
		mpv::setPropertyAsync(mpvHandle(), "sub-font-size", style.size);
		mpv::setPropertyAsync(mpvHandle(), "sub-color", style.color);
		mpv::setPropertyAsync(mpvHandle(), "sub-back-color", style.backgroundColor);
	}

	int CrowPlayer::volumeMax() const
	{
		return mpv::getProperty(mpvHandle(), "volume-max").toInt();
	}

	long long CrowPlayer::duration() const
	{
		return mpv::getProperty(mpvHandle(), "duration").toInt();
	}

	bool CrowPlayer::isPlaying() const
	{
		return !isPaused();
	}

	bool CrowPlayer::isPaused() const
	{
		return mpv::getProperty(mpvHandle(), "pause").toBool();
	}

	void CrowPlayer::showContextMenu(const QPoint& position)
	{
		QMenu contextMenu(tr("Video Tools"), this);

		QAction copySubAction("Copy Subtitle", this);
		QAction copySecondarySubAction("Copy Secondary Subtitle", this);

		connect(&copySubAction, &QAction::triggered, [&]() {
			QClipboard* clipboard = qApp->clipboard();
			clipboard->setText(getSubtitle());
		});

		connect(&copySecondarySubAction, &QAction::triggered, [&]() {
			QClipboard* clipboard = qApp->clipboard();
			clipboard->setText(getSecondarySubtitle());
		});

		contextMenu.addAction(&copySubAction);
		contextMenu.addAction(&copySecondarySubAction);
		contextMenu.exec(mapToGlobal(position));
	}

	void CrowPlayer::handleRepeatOnPositionChange(double position)
	{
		if (!m_IsLooping || m_StartLoop == m_EndLoop || m_EndLoop == 0)
			return;

		if (position >= m_EndLoop)
			seekAbsolute(m_StartLoop);
	}
}