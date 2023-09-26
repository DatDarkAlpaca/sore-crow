#include "pch.h"
#include "stylesheet_handler.h"
#include "utils/message_box_utils.h"
#include "core/filesystem/filesystem_handler.h"
#include "core/settings/settings_handler.h"

namespace sore
{
	StylesheetHandler::StylesheetHandler(QObject* parent)
		: QObject(parent)
	{

	}

	void StylesheetHandler::initialize()
	{
		if(!s_Instance)
			s_Instance = new StylesheetHandler;
		
		stylesheet = std::make_unique<acss::QtAdvancedStylesheet>();
		
		initializeDefaultStylesheet();

		updateSettings();
	}

	void StylesheetHandler::initializePlayerStyles()
	{
		s_Instance->setSubtitleFontStyles(s_Instance->getSubtitleFontStyles());
	}

	bool StylesheetHandler::styleExists(const QString& styleName)
	{
		bool found = false;
		for (const auto& style : stylesheet->styles())
		{
			if (styleName == style)
			{
				found = true;
				break;
			}
		}

		return found;
	}

	bool StylesheetHandler::themeExists(const QString& themeName)
	{
		bool found = false;
		for (const auto& theme : stylesheet->themes())
		{
			if (themeName == theme)
			{
				found = true;
				break;
			}
		}

		return found;
	}

	void StylesheetHandler::selectStyle(const QString& styleName)
	{
		if (!styleExists(styleName))
		{
			messageBox("Stylesheet Handler", "This style does not exist.");
			return;
		}

		stylesheet->setCurrentStyle(styleName);
	}

	void StylesheetHandler::selectTheme(const QString& themeName)
	{
		if (!themeExists(themeName))
		{
			messageBox("Stylesheet Handler", "This theme does not exist.");
			return;
		}

		stylesheet->setCurrentTheme(themeName);
	}

	void StylesheetHandler::applyStylesheet()
	{
		stylesheet->updateStylesheet();
		qApp->setStyleSheet(stylesheet->styleSheet());

		auto& settings = SettingsHandler::settings;
		settings->setValue("styles/themes/selected_style", stylesheet->currentStyle());
		settings->setValue("styles/themes/selected_theme", stylesheet->currentTheme());
	}

	void StylesheetHandler::setSubtitleOverride(bool enabled)
	{
		auto& settings = SettingsHandler::settings;
		settings->setValue("styles/subtitles/override_styles", enabled);
		
		emit overrideStylesChanged(enabled);
	}

	bool StylesheetHandler::getSubtitleOverride()
	{
		auto& settings = SettingsHandler::settings;
		return settings->value("styles/subtitles/override_styles", false).toBool();
	}

	SubtitleFontStyles StylesheetHandler::getSubtitleFontStyles()
	{
		auto& settings = SettingsHandler::settings;

		QString fontFamily = settings->getString("styles/subtitles/primary/font_family");
		int fontSize = settings->value("styles/subtitles/primary/font_size").toInt();
		QString fontColor = settings->getString("styles/subtitles/primary/font_color");
		QString backgroundColor = settings->getString("styles/subtitles/primary/font_background_color");

		return {
			fontFamily,
			fontSize,
			fontColor,
			backgroundColor
		};
	}

	void StylesheetHandler::setSubtitleFontStyles(const SubtitleFontStyles& styles)
	{
		auto& settings = SettingsHandler::settings;

		settings->setValue("styles/subtitles/primary/font_family", styles.family);
		settings->setValue("styles/subtitles/primary/font_size", styles.size);
		settings->setValue("styles/subtitles/primary/font_color", styles.color);
		settings->setValue("styles/subtitles/primary/font_background_color", styles.backgroundColor);

		emit subtitleStyleChanged(styles);
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
			messageBox("Stylesheet Handler", "No style or theme was found.");
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