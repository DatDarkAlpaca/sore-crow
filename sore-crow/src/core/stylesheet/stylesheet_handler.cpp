#include "pch.h"
#include "stylesheet_handler.h"
#include "utils/message_box_utils.h"
#include "core/filesystem/filesystem_handler.h"
#include "core/settings/settings_handler.h"

namespace sore
{
	void StylesheetHandler::initialize()
	{
		stylesheet = std::make_unique<acss::QtAdvancedStylesheet>();

		initializeDefaultStylesheet();

		updateSettings();
	}

	void StylesheetHandler::initializeDefaultStylesheet()
	{
		auto& settings = SettingsHandler::settings;

		std::string absoluteStylePath = FilesystemHandler::getAbsolutePath("paths/style_path").string();
		std::string absoluteStyleOutPath = FilesystemHandler::getAbsolutePath("paths/style_output_path").string();

		stylesheet->setStylesDirPath(absoluteStylePath.c_str());
		stylesheet->setOutputDirPath(absoluteStyleOutPath.c_str());

		auto currentStyle = settings->value("styles/themes/selected_style").toString();
		auto currentTheme = settings->value("styles/themes/selected_theme").toString();

		if (currentStyle.isEmpty() || currentTheme.isEmpty())
		{
			errorBox("Stylesheet Handler", "No style or theme was found.");
			return;
		}

		stylesheet->setCurrentStyle(currentStyle);
		stylesheet->setCurrentTheme(currentTheme);
		stylesheet->updateStylesheet();

		qApp->setStyleSheet(stylesheet->styleSheet());
	}

	void StylesheetHandler::updateSettings()
	{
		auto& settings = SettingsHandler::settings;
		settings->setValue("styles/themes/is_theme_dark", stylesheet->isCurrentThemeDark());
	}
}