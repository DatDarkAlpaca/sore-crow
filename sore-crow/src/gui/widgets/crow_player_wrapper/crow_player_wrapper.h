#pragma once
#include <Qt>
#include <QShortcut>
#include <QHoverEvent>
#include "core/handlers.h"
#include "gui/widgets/crow_player/crow_player.h"
#include "gui/widgets/player_controls/player_controls.h"
#include "gui/widgets/selectable_subtitles/selectable_subtitles.h"

namespace sore
{
	class CrowPlayerWrapper : public CrowPlayer
	{
		Q_OBJECT

	public:
		CrowPlayerWrapper(QWidget* parent = nullptr);

	protected:
		bool event(QEvent* event) override;

		bool eventFilter(QObject* object, QEvent* event) override;

	private:
		void onHoverEnter(QHoverEvent* hoverEvent);

		void onHoverLeave(QHoverEvent* hoverEvent);

		void onHoverMove(QHoverEvent* hoverEvent);

	private:
		void setupFullscreen();

		void toggleFullscreen();
		
		void enterFullscreen();

		void leaveFullscreen();

	private:
		void setupTimer();

		void onUserInactive();

	private:
		void setupShortcuts();

		void handleShortcuts(const QString& shortcut);

	private:
		void setupUI();

	signals:
		void keyPressed(int keyCode);

	public:
		PlayerControls* controls = nullptr;
		SelectableSubtitles* subtitles = nullptr;

	private:
		QTimer* m_HideControlTimer;
		bool m_UserInactive = false;

		static inline constexpr int SlideDuration = 500;
	};
}