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

		return projectData;
	}

	std::optional<ProjectData> CrowApp::onCreateProject()
	{
		namespace fs = std::filesystem;

		CreateProjectDialog dialog;
		if (dialog.exec() != QDialog::Accepted)
			return {};

		auto dialogData = dialog.getData();

		ProjectData projectData;
		projectData.projectName = dialogData.projectName;
		projectData.rootFolder = dialogData.projectRootFolder;

		// Check for existing supported episode and subtitle folders:
		fs::path episodesFolder;
		for (const auto filepath : getFilesInDir(dialogData.projectRootFolder))
		{
			QFileInfo fileInfo(filepath.c_str());
			if (!fileInfo.isDir())
				continue;

			if (isValidEpisodesDirectory(filepath))
			{
				fs::path episodesPath(filepath);
				episodesFolder = episodesPath;
			}
		}

		// Create these folders if not available:
		if (episodesFolder.empty())
		{
			fs::path rootPath(projectData.rootFolder);
			fs::path episodesFolderPath(Macros::AcceptableEpisodeFolderNames.front());
			episodesFolder = rootPath / episodesFolderPath;
			fs::create_directory(episodesFolder);
		}

		// Move all episodes to the appropriate folder:
		for (const auto& episodePath : dialogData.episodePaths)
		{
			fs::path previousFilepath(episodePath);
			fs::rename(previousFilepath, episodesFolder / previousFilepath.filename());
		}

		projectData.episodeFolderName = episodesFolder.string();

		// Get files inside folders:
		auto correctedProjectFiles = getFilesInDir(projectData.episodeFolderName);
		auto correctedSubtitleFiles = getFilesInDir(projectData.episodeFolderName);

		projectData.sourceMetadata.id = generateUUID();

		// Iterate over episodes and subtitles to generate metadata:
		for (size_t i = 0; i < correctedProjectFiles.size(); ++i)
		{
			EpisodeMetadata episode;
			episode.id = generateUUID();
			episode.filename = correctedProjectFiles[i];
			projectData.sourceMetadata.episodes.push_back(episode);
		}

		createProjectFile(projectData);

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