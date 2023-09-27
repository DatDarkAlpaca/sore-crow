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
		CrowPlayerWrapper(QWidget* parent = nullptr);

	protected:
		bool event(QEvent* event) override;

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
		void setupUI();

	public:
		PlayerControls* controls = nullptr;

	private:
		QTimer* m_HideControlTimer;
		bool m_UserInactive = false;

		static inline constexpr int SlideDuration = 500;
	};
}