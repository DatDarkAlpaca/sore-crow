#pragma once
#include "gui/view/crow_window/crow_window.h"
#include "gui/view/splash_window/splash_window.h"

#include <QtAdvancedStylesheet.h>

namespace sore
{
	class MainController
	{
	public:
		MainController();

		~MainController();

	public:
		void execute();

	private slots:
		ProjectData onOpenProject();

		ProjectData onCreateProject();

	private:
		void configureStylesheet();
		
		void configureActions();

	private:
		acss::QtAdvancedStylesheet* m_Stylesheet;

	private:
		SplashWindow m_SplashWindow;
		CrowWindow m_CrowWindow;
	};
}