#pragma once
#include "gui/view/crow_window/crow_window.h"
#include "gui/view/splash_window/splash_window.h"

namespace sore
{
	class MainController
	{
	public:
		MainController();

	public:
		void execute();

	private slots:
		ProjectData onOpenProject();

		ProjectData onCreateProject();
		
		void configureActions();

	private:
		SplashWindow m_SplashWindow;
		CrowWindow m_CrowWindow;
	};
}