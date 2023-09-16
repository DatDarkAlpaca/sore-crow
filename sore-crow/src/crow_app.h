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
		std::optional<ProjectData> onOpenProject();

		std::optional<ProjectData> onCreateProject();

	private:
		void configureSettings();

		void configureViews();

		void configureStylesheets();
		
		void configureActions();

	private:
		CrowWindow* m_CrowWindow;
		SplashWindow* m_SplashWindow;

	private:
		acss::QtAdvancedStylesheet* m_Stylesheet;

	private:
		ProjectData m_ProjectData;
		bool m_DirtyProject = false;
	};
}