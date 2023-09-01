#pragma once
#include "gui/view/crow_window.h"
#include "gui/view/splash_window.h"
#include "gui/data/crow_window_data.h"

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
		ProjectData handleOpenProject();

		ProjectData handleCreateProject();

	private:
		void configureStylesheet();
		
		void configureActions();

	private:
		acss::QtAdvancedStylesheet* m_Stylesheet;

	private:
		CrowWindowData m_CrowWindowData;
		SplashWindow m_SplashWindow;
		CrowWindow m_CrowWindow;
	};
}