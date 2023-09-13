#include "pch.h"
#include "crow_app.h"

#include "core/global_data.h"
#include "core/logger/logger.h"
#include "core/project/project_data.h"
#include "core/plugin/dictionary_plugin.h"

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
		configureData();

		configureStylesheets();

		configureActions();

		CrownsoleLogger::setConsoleWidget(m_CrowWindow.ui.crownsole);
	}

	void CrowApp::execute()
	{
		m_SplashWindow.show();
	}

	std::optional<ProjectData> CrowApp::onOpenProject()
	{
		namespace fs = std::filesystem;
		std::string projectFilepath = openProjectFileDialog();

		if (projectFilepath.empty())
			return std::nullopt;

		auto projectData = getProjectData(projectFilepath);
		if (!projectData.has_value())
		{
			QString error("This project file is invalid or corrupted.");
			error = error.arg(projectFilepath.c_str());
			errorBox(error.toStdString());

			return std::nullopt;
		}

		auto parentPath = fs::path(projectFilepath).parent_path();
		ProjectDirectory = parentPath.string();
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
		ProjectDirectory = projectFolderPath.string();

		// Project Header Creation:
		ProjectData projectData;
		projectData.header.projectName = dialogData.projectName;
		projectData.header.projectVersion = Macros::version;

		// Moves episodes:
		if (dialogData.moveEpisodes)
		{
			createEpisodesFolder(projectFolderPath, Macros::DefaultEpisodesFolderName);

			episodeFilepaths = moveToEpisodesFolder(dialogData.episodePaths, projectFolderPath / Macros::DefaultEpisodesFolderName);
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

		createProjectFile(projectData, projectFolderPath.string());
		return projectData;
	}

	void CrowApp::configureActions()
	{
		// [Splash Window] Open Project:
		QObject::connect(m_SplashWindow.ui.openProjectBtn, &QPushButton::released, [&]() {
			auto projectData = onOpenProject();
			if (!projectData.has_value())
				return;

			m_CrowWindow.clearData();
			m_CrowWindow.updateData(projectData.value());

			m_SplashWindow.hide();
			m_CrowWindow.show();
			});

		// [Splash Window] Create Project:
		QObject::connect(m_SplashWindow.ui.createProjectBtn, &QPushButton::released, [&]() {
			auto projectData = onCreateProject();
			if (!projectData.has_value())
				return;

			m_CrowWindow.clearData();
			m_CrowWindow.updateData(projectData.value());

			m_SplashWindow.hide();
			m_CrowWindow.show();
			});

		// [Main Window] Open Project:
		QObject::connect(m_CrowWindow.ui.actionOpenProject, &QAction::triggered, [&]() {
			auto projectData = onOpenProject();
			if (!projectData.has_value())
				return;

			m_CrowWindow.clearData();
			m_CrowWindow.updateData(projectData.value());
			});

		// [Main Window] Create Project:
		QObject::connect(m_CrowWindow.ui.actionNewProject, &QAction::triggered, [&]() {
			auto projectData = onCreateProject();
			if (!projectData.has_value())
				return;

			m_CrowWindow.clearData();
			m_CrowWindow.updateData(projectData.value());
			});
	}

	void CrowApp::configureData()
	{
		Data::initialize();
	}

	void CrowApp::configureStylesheets()
	{
		DictionaryPlugin plugin("test_plugin");
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