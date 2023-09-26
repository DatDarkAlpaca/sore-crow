#include "pch.h"
#include "constants.h"
#include "settings_handler.h"

namespace sore
{
	void SettingsHandler::initialize()
	{
		namespace fs = std::filesystem;

		settingsPath = qApp->applicationDirPath() + "/settings.ini";
		settings = std::make_unique<CrowSettings>(settingsPath, QSettings::IniFormat);

		if (fs::is_regular_file(settingsPath.toStdString()))
			return;

		initializeDefaultSettings();
	}

	void SettingsHandler::save()
	{
		settings->sync();
	}

	void SettingsHandler::initializeDefaultSettings()
	{
		settings->setValue("version", ProjectVersion);

		settings->setValue("project/extension", "prj");
		settings->setValue("project/default_episodes_folder_name", "episodes");
		settings->setValue("project/supported/video_formats", "mkv,mp4");
		settings->setValue("project/supported/subtitle_formats", "srt,ass,sbv");
		settings->setValue("project/directory", "");
		
		settings->setValue("paths/resources_folder", "res");
		settings->setValue("paths/style_path", "res/styles");
		settings->setValue("paths/style_output_path", "res/output");
		settings->setValue("paths/plugin_path", "plugins");

		settings->setValue("mpv/enable_config", "yes");
		settings->setValue("mpv/config_path", "res/config.conf");

		settings->setValue("styles/themes/selected_style", "crow_material");
		settings->setValue("styles/themes/selected_theme", "dark_purple");
		settings->setValue("styles/themes/is_theme_dark", true);

		settings->setValue("styles/subtitles/override_styles", false);
		settings->setValue("styles/subtitles/primary/font_family", "segoe ui");
		settings->setValue("styles/subtitles/primary/font_size", 12);
		settings->setValue("styles/subtitles/primary/font_color", "#FFFFFFFF");
		settings->setValue("styles/subtitles/primary/font_background_color", "#000000C0");

		settings->sync();
	}
}