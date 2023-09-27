#include "pch.h"
#include "crow_app.h"
#include "core/handlers.h"

namespace sore
{
	CrowApp::CrowApp(int argc, char** argv)
		: QApplication(argc, argv)
	{
		setupAttributes();

		setupHandlers();

		setlocale(LC_NUMERIC, "C");
	}

	void CrowApp::setupAttributes()
	{
		QApplication::setOrganizationName("DatDarkAlpaca");
		QApplication::setApplicationName("SoreCrow");

		QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
		QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
		QCoreApplication::setAttribute(Qt::AA_CompressHighFrequencyEvents);
	}

	void CrowApp::setupHandlers()
	{
		SettingsHandler::initialize();
		FilesystemHandler::initialize();
		StylesheetHandler::initialize();
	}

	void preInitializeApp()
	{
		qApp->setAttribute(Qt::AA_ShareOpenGLContexts);
	}
}