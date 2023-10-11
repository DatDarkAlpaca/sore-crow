#include "pch.h"
#include "crow_player_wrapper.h"


namespace sore
{
	CrowPlayerWrapper::CrowPlayerWrapper(QWidget* parent)
	{
		setupUI();
		setupTimer();
		setupFullscreen();
		setupShortcuts();
	}

	bool CrowPlayerWrapper::event(QEvent* event)
	{
		switch (event->type())
		{
			case QEvent::HoverEnter:
			{
				onHoverEnter(static_cast<QHoverEvent*>(event));
				return true;
			}

			case QEvent::HoverLeave:
			{
				onHoverLeave(static_cast<QHoverEvent*>(event));
				return true;
			}

			case QEvent::HoverMove:
			{
				onHoverEnter(static_cast<QHoverEvent*>(event));
				return true;
			}
		}

		return QWidget::event(event);
	}

	bool CrowPlayerWrapper::eventFilter(QObject* object, QEvent* event)
	{
		if (event->type() != QEvent::KeyPress)
			return false;

		if (!isVisible())
			return false;

		auto keyEvent = static_cast<QKeyEvent*>(event);
		emit keyPressed(keyEvent->key());
		return true;
	}

	void CrowPlayerWrapper::onHoverEnter(QHoverEvent* hoverEvent)
	{
		controls->show();
		m_UserInactive = false;

		qApp->restoreOverrideCursor();

		m_HideControlTimer->start();
	}

	void CrowPlayerWrapper::onHoverLeave(QHoverEvent* hoverEvent)
	{
		controls->hide();
	}

	void CrowPlayerWrapper::onHoverMove(QHoverEvent* hoverEvent)
	{
		onHoverEnter(hoverEvent);
	}

	void CrowPlayerWrapper::setupFullscreen()
	{
		auto& settings = SettingsHandler::settings;

		// Toggle Fullscreen:
		auto shortcutStrings = settings->getStrings("shortcuts/toggle_fullscreen");
		for (const auto& shortcutString : shortcutStrings)
		{
			QShortcut* shortcut = new QShortcut(QKeySequence(shortcutString), this);
			connect(shortcut, &QShortcut::activated, this, &CrowPlayerWrapper::toggleFullscreen);
		}

		// Leave Fullscreen:
		shortcutStrings = settings->getStrings("shortcuts/exit_fullscreen");
		for (const auto& shortcutString : shortcutStrings)
		{
			QShortcut* shortcut = new QShortcut(QKeySequence(shortcutString), this);
			connect(shortcut, &QShortcut::activated, this, &CrowPlayerWrapper::leaveFullscreen);
		}
	}

	void CrowPlayerWrapper::toggleFullscreen()
	{
		if (isFullScreen())
			leaveFullscreen();
		else
			enterFullscreen();
	}

	void CrowPlayerWrapper::enterFullscreen()
	{
		if (isFullScreen())
			return;

		setWindowFlags(Qt::Window);
		setWindowState(Qt::WindowFullScreen);
		show();
	}

	void CrowPlayerWrapper::leaveFullscreen()
	{
		if (!isFullScreen())
			return;

		setWindowState(Qt::WindowActive);
		setWindowFlags(Qt::Widget);
		show();
	}

	void CrowPlayerWrapper::setupTimer()
	{
		m_HideControlTimer = new QTimer;
		m_HideControlTimer->setInterval(1000);

		connect(m_HideControlTimer, &QTimer::timeout, this, &CrowPlayerWrapper::onUserInactive);
	}

	void CrowPlayerWrapper::onUserInactive()
	{
		if (m_UserInactive)
			return;

		m_UserInactive = true;
		controls->hide();

		if (isFullScreen())
		{
			QCursor cursor(Qt::BlankCursor);
			qApp->setOverrideCursor(cursor);
		}
	}

	void CrowPlayerWrapper::setupShortcuts()
	{
		qApp->installEventFilter(this);
		
		connect(this, &CrowPlayerWrapper::keyPressed, [&](int keyCode) {
			auto& settings = SettingsHandler::settings;
			settings->beginGroup("shortcuts/video");

			for (const auto& groupKey : settings->childKeys())
			{
				QString shortcutString = settings->value(groupKey).toString();

				QKeySequence sequence(shortcutString);
				if (sequence[0].key() == keyCode)
				{
					handleShortcuts(groupKey);
					break;
				}
			}
			settings->endGroup();
		});
	}

	void CrowPlayerWrapper::handleShortcuts(const QString& shortcut)
	{
		// TODO: use a hash table if this gets troublesome.

		if (shortcut == "play_pause")
			this->togglePlay();

		else if (shortcut == "seek_back")
			this->seek(-1000, SeekFlag::SEEK_RELATIVE);

		else if (shortcut == "seek_forward")
			this->seek(1000, SeekFlag::SEEK_RELATIVE);
	}

	void CrowPlayerWrapper::setupUI()
	{
		setAttribute(Qt::WA_Hover);
		controls = new PlayerControls(this);
		subtitles = new SelectableSubtitles(this);

		QVBoxLayout* layout = new QVBoxLayout(this);
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(subtitles);

		QVBoxLayout* playerLayout = new QVBoxLayout(subtitles);
		playerLayout->addWidget(controls, 0, Qt::AlignBottom);
	}
}