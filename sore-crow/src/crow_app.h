#pragma once
#include <QtWidgets/QApplication>
#include <QtAdvancedStylesheet.h>

#include "core/project/project_data.h"
#include "gui/view/crow_window/crow_window.h"
#include "gui/view/splash_window/splash_window.h"

namespace sore
{
	class CrowApp : public QApplication
	{
	public:
		CrowApp(int argc, char** argv);

	public:
		void execute();

	private:
		ProjectData onOpenProject();

		ProjectData onCreateProject();

		void configureActions();

	private:
		void configureStylesheets();

	private:
		CrowWindow m_CrowWindow;
		SplashWindow m_SplashWindow;
		acss::QtAdvancedStylesheet* m_Stylesheet;
	};
}