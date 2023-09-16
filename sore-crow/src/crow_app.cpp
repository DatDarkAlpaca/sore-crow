#include "pch.h"
#include "crow_app.h"

#include "crow_settings.h"
#include "core/logger/logger.h"
#include "core/project/project_data.h"

#include "utils/dialog.h"
#include "utils/uuid_utils.h"
#include "utils/message_box.h"
#include "utils/filesystem_utils.h"
#include "utils/project_utils.h"
#include "gui/view/dialog/create_project_dialog.h"

namespace sore
{
	CrowApp::CrowApp(int argc, char** argv)
		: QApplication(argc, argv)
	{
		QApplication::setOrganizationName("DatDarkAlpaca");
		QApplication::setApplicationName("SoreCrow");

		std::filesystem::current_path(qApp->applicationDirPath().toStdString());

		configureSettings();

		configureViews();

		configureStylesheets();

		configureActions();

		CrownsoleLogger::setConsoleWidget(m_CrowWindow->ui.crownsole);
	}

	void CrowApp::execute()
	{
		m_SplashWindow->show();
	}

	std::optional<ProjectData> CrowApp::onOpenProject()
	{
		namespace fs = std::filesystem;
		std::string projectFilepath = openProjectFileDialog();

		if (projectFilepath.empty())
			return std::nullopt;

		ProjectData projectData;
		bool result = projectData.open(projectFilepath);
		if (!result)
		{
			QString error("This project file is invalid or corrupted: %1");
			error = error.arg(projectFilepath.c_str());
			errorBox(error.toStdString());

			return std::nullopt;
		}

		auto projectFolderPath = fs::path(projectFilepath).parent_path();
		settings->setValue("paths/project_directory", projectFolderPath.string().c_str());

		return projectData;
	}

	std::optional<ProjectData> CrowApp::onCreateProject()
	{
		namespace fs = std::filesystem;

		// Dialog:
		CreateProjectDialog dialog;
		if (dialog.exec() != QDialog::Accepted)
			return std::nullopt;

		auto dialogData = dialog.getData();
		std::vector<std::string> episodeFilepaths = dialogData.episodePaths;

		// Folder Path:
		fs::path projectFolderPath(dialogData.projectFilepath);
		settings->setValue("paths/project_directory", projectFolderPath.string().c_str());

		// Project Filepath:
		std::string projectFileName = dialogData.projectName + "." + settings->value("project/extension").toString().toStdString();
		fs::path projectFilepath = projectFolderPath / projectFileName;

		// Project Header Creation:
		ProjectData projectData;
		projectData.projectFilepath = projectFilepath.string();
		projectData.header.projectName = dialogData.projectName;
		projectData.header.projectVersion = settings->value("version").toString().toStdString();

		// Moves episodes:
		if (dialogData.moveEpisodes)
		{
			auto episodesFolderName = settings->value("project/default_episodes_folder_name").toString().toStdString();
			createEpisodesFolder(projectFolderPath, episodesFolderName);

			episodeFilepaths = moveToEpisodesFolder(dialogData.episodePaths, projectFolderPath / episodesFolderName);
		}

		// Create episode data:
		for (const auto& episodePath : episodeFilepaths)
		{
			fs::path relative;
			if (!dialogData.moveEpisodes)
				relative = fs::relative(projectFolderPath, fs::path(episodePath).parent_path());
			else
				relative = fs::relative(fs::path(episodePath).parent_path(), projectFolderPath);

			fs::path episodeFilepath = fs::path(episodePath).filename();

			EpisodeData episodeData;
			episodeData.filepath = (relative / episodeFilepath).generic_string();
			projectData.mediaData.episodeData.push_back(episodeData);
		}

		projectData.save();
		return projectData;
	}

	void CrowApp::configureSettings()
	{
		namespace fs = std::filesystem;
		const QString settingsPath = qApp->applicationDirPath() + "/settings.ini";

		settings = std::make_unique<QSettings>(settingsPath, QSettings::IniFormat);

		if (fs::is_regular_file(settingsPath.toStdString()))
			return;

		settings->setValue("version", "0.1");

		settings->setValue("project/extension", "prj");
		settings->setValue("project/default_episodes_folder_name", "episodes");
		settings->setValue("project/supported/video_formats", "mkv,mp4");

		settings->setValue("paths/resources_folder", "/res");
		settings->setValue("paths/style_path", "/res/styles");
		settings->setValue("paths/style_output_path", "/res/output");
		settings->setValue("paths/plugin_path", "/plugins");
		settings->setValue("paths/working_directory", "");
		settings->setValue("paths/project_directory", "");

		settings->setValue("styles/themes/is_theme_dark", true);
		settings->setValue("styles/subtitles/point_size", 20);
		settings->sync();
	}

	void CrowApp::configureViews()
	{
		m_CrowWindow = new CrowWindow;
		m_SplashWindow = new SplashWindow;
	}
	
	void CrowApp::configureStylesheets()
	{
		m_Stylesheet = new acss::QtAdvancedStylesheet();

		auto absoluteStylePath = qApp->applicationDirPath() + settings->value("paths/style_path").toString();
		auto absoluteStyleOutPath = qApp->applicationDirPath() + settings->value("paths/style_output_path").toString();

		m_Stylesheet->setStylesDirPath(absoluteStylePath);
		m_Stylesheet->setOutputDirPath(absoluteStyleOutPath);
		m_Stylesheet->setCurrentStyle("crow_material");

		m_Stylesheet->setCurrentTheme("dark_purple");
		m_Stylesheet->updateStylesheet();

		settings->setValue("styles/themes/is_theme_dark", m_Stylesheet->isCurrentThemeDark());
		qApp->setStyleSheet(m_Stylesheet->styleSheet());
	}

	void CrowApp::configureActions()
	{
		// [CrowApp] On Close:
		QObject::connect(this, &CrowApp::aboutToQuit, [&]() {
			long long lastPosition = m_CrowWindow->currentPosition();
			if(lastPosition > 0)
				m_ProjectData.header.lastEpisodePosition = lastPosition;

			m_ProjectData.save();
		});

		// [Splash Window] Open Project:
		QObject::connect(m_SplashWindow->ui.openProjectBtn, &QPushButton::released, [&]() {
			auto projectData = onOpenProject();
			if (!projectData.has_value())
				return;

			if (m_DirtyProject)
			{
				m_ProjectData.save();
				m_DirtyProject = false;
			}
			m_ProjectData = projectData.value();

			m_CrowWindow->clearData();
			m_CrowWindow->updateData(m_ProjectData);

			m_SplashWindow->hide();
			m_CrowWindow->show();
		});

		// [Splash Window] Create Project:
		QObject::connect(m_SplashWindow->ui.createProjectBtn, &QPushButton::released, [&]() {
			auto projectData = onCreateProject();
			if (!projectData.has_value())
				return;

			if (m_DirtyProject)
			{
				m_ProjectData.save();
				m_DirtyProject = false;
			}

			m_ProjectData = projectData.value();

			m_CrowWindow->clearData();
			m_CrowWindow->updateData(m_ProjectData);

			m_SplashWindow->hide();
			m_CrowWindow->show();
		});

		// [Main Window] Open Project:
		QObject::connect(m_CrowWindow->ui.actionOpenProject, &QAction::triggered, [&]() {
			auto projectData = onOpenProject();
			if (!projectData.has_value())
				return;

			if (m_DirtyProject)
			{
				m_ProjectData.save();
				m_DirtyProject = false;
			}

			m_ProjectData = projectData.value();

			m_CrowWindow->clearData();
			m_CrowWindow->updateData(m_ProjectData);
		});

		// [Main Window] Create Project:
		QObject::connect(m_CrowWindow->ui.actionNewProject, &QAction::triggered, [&]() {
			auto projectData = onCreateProject();
			if (!projectData.has_value())
				return;

			if (m_DirtyProject)
			{
				m_ProjectData.save();
				m_DirtyProject = false;
			}

			m_ProjectData = projectData.value();

			m_CrowWindow->clearData();
			m_CrowWindow->updateData(m_ProjectData);
		});
	
		// [Main Window] Request Project Change:
		QObject::connect(m_CrowWindow, &CrowWindow::projectMustChange, [&](const std::string& section, const std::string& field, const nlohmann::json& object) {
			m_ProjectData.update(section, field, object);
			m_DirtyProject = true;
		});
	}
}