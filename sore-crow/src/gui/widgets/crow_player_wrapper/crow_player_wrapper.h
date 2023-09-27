#pragma once
#include <Qt>
#include <QShortcut>
#include <QHoverEvent>
#include "core/handlers.h"
#include "gui/widgets/crow_player/crow_player.h"
#include "gui/widgets/player_controls/player_controls.h"

namespace sore
{
	class CrowPlayerWrapper : public CrowPlayer
	{
	public:
		CrowPlayerWrapper(QWidget* parent = nullptr)
			: CrowPlayer(parent)
		{
			setupUI();
			setupTimer();
			setupFullscreen();
		}

	protected:
		bool event(QEvent* event) override
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

	private:
		void onHoverEnter(QHoverEvent* hoverEvent)
		{
			controls->show();
			m_UserInactive = false;

			QCursor cursor(Qt::ArrowCursor);
			qApp->setOverrideCursor(cursor);

			m_HideControlTimer->start();
		}

		void onHoverLeave(QHoverEvent* hoverEvent)
		{
			controls->hide();
		}

		void onHoverMove(QHoverEvent* hoverEvent)
		{
			onHoverEnter(hoverEvent);
		}

	private:
		void setupFullscreen()
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

		void toggleFullscreen()
		{
			if (isFullScreen())
				leaveFullscreen();
			else 
				enterFullscreen();
		}

		void enterFullscreen()
		{
			if (isFullScreen())
				return;

			setWindowFlags(Qt::Window);
			setWindowState(Qt::WindowFullScreen);
			show();
		}

		void leaveFullscreen()
		{
			if (!isFullScreen())
				return;

			setWindowState(Qt::WindowActive);
			setWindowFlags(Qt::Widget);
			show();
		}

	private:
		void setupTimer()
		{
			m_HideControlTimer = new QTimer;
			m_HideControlTimer->setInterval(1000);

			connect(m_HideControlTimer, &QTimer::timeout, this, &CrowPlayerWrapper::onUserInactive);
		}

		void onUserInactive()
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

	private:
		void setupUI()
		{
			setAttribute(Qt::WA_Hover);
			controls = new PlayerControls(this);

			QVBoxLayout* layout = new QVBoxLayout(this);
			layout->addWidget(controls);
			layout->setAlignment(controls, Qt::AlignBottom);
		}

	public:
		PlayerControls* controls = nullptr;

	private:
		QTimer* m_HideControlTimer;
		bool m_UserInactive = false;

		static inline constexpr int SlideDuration = 500;
	};
}