#include "pch.h"
#include "crow_app.h"
#include "core/global_data.h"
#include "core/plugin/dictionary_plugin.h"

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
		DictionaryPlugin plugin("test_plugin");
		plugin.setDictionaryPath("C:/Users/paulo/Projects/sore-crow/sore-crow/plugins/jmdict_importer/JMdict_e.xml");
		plugin.execute();

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