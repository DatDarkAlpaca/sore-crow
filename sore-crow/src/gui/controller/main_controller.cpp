#include "pch.h"
#include "main_controller.h"
#include "gui/data/global_data.h"

#include "utils/dialog.h"
#include "utils/uuid_utils.h"
#include "utils/filesystem_utils.h"
#include "core/project/project_data.h"
#include "gui/view/dialog/create_project_dialog.h"

namespace sore
{
	MainController::MainController()
	{
		configureStylesheet();

		configureActions();
	}

	MainController::~MainController()
	{
		delete m_Stylesheet;
	}

	void MainController::execute()
	{
		m_SplashWindow.show();
	}

	ProjectData MainController::handleOpenProject()
	{
		std::string projectFile = openProjectFileDialog();
		if (projectFile.empty())
		{
			qDebug() << "Invalid project filepath.";
			return {};
		}

		ProjectData projectData = getProjectData(projectFile);
		if (!projectData.valid())
		{
			qDebug() << "The selected document is not a valid project file.";
			return {};
		}

		return projectData;
	}

	ProjectData MainController::handleCreateProject()
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

	void MainController::configureStylesheet()
	{
		m_Stylesheet = new acss::QtAdvancedStylesheet();

		// Todo: organize the style and output files in a separate handler.
		m_Stylesheet->setStylesDirPath("C:/Users/paulo/Projects/sore-crow/sore-crow/resources/styles");
		m_Stylesheet->setOutputDirPath("C:/Users/paulo/Projects/sore-crow/sore-crow/resources/output");
		m_Stylesheet->setCurrentStyle("crow_material");

		m_Stylesheet->setCurrentTheme("dark_purple");
		m_Stylesheet->updateStylesheet();

		globalData.isStylesheetDark = m_Stylesheet->isCurrentThemeDark();

		qApp->setStyleSheet(m_Stylesheet->styleSheet());

	}

	void MainController::configureActions()
	{
		// [Splash Window] Open Project:
		QObject::connect(m_SplashWindow.ui.openProjectBtn, &QPushButton::released, [&]() {
			auto projectData = handleOpenProject();
			if (!projectData.valid())
				return;

			m_CrowWindow.clearData();
			m_CrowWindow.updateData(projectData);

			m_SplashWindow.hide();
			m_CrowWindow.show();
		});

		// [Splash Window] Create Project:
		QObject::connect(m_SplashWindow.ui.createProjectBtn, &QPushButton::released, [&]() {
			auto projectData = handleCreateProject();
			if (!projectData.valid())
				return;

			m_CrowWindow.clearData();
			m_CrowWindow.updateData(projectData);

			m_SplashWindow.hide();
			m_CrowWindow.show();
		});

		// [Main Window] Open Project:
		QObject::connect(m_CrowWindow.ui.actionOpenProject, &QAction::triggered, [&]() {
			auto projectData = handleOpenProject();
			if (!projectData.valid())
				return;

			m_CrowWindow.clearData();
			m_CrowWindow.updateData(projectData);
		});
	}
}