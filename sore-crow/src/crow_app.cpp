#include "pch.h"
#include "crow_app.h"
#include "core/global_data.h"
#include "core/plugin/plugin_handler.h"

namespace sore
{
	CrowApp::CrowApp(int argc, char** argv)
		: QApplication(argc, argv)
	{
		configureStylesheets();
	}

	void CrowApp::configureStylesheets()
	{
		Data::initialize();
		m_Stylesheet = new acss::QtAdvancedStylesheet();

		m_Stylesheet->setStylesDirPath(StylePath.c_str());
		m_Stylesheet->setOutputDirPath(StyleOutputPath.c_str());
		m_Stylesheet->setCurrentStyle("crow_material");

		m_Stylesheet->setCurrentTheme("dark_purple");
		m_Stylesheet->updateStylesheet();

		globalData.isStylesheetDark = m_Stylesheet->isCurrentThemeDark();

		qApp->setStyleSheet(m_Stylesheet->styleSheet());
	}
}