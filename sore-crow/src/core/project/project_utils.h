#pragma once
#include <optional>
#include "project_data.h"
#include "core/handlers.h"
#include "utils/dialog_utils.h"
#include "utils/message_box_utils.h"
#include "gui/view/create_project_dialog/create_project_dialog.h"

namespace sore
{
	inline std::optional<ProjectData> openProjectFromDialog()
	{
		QString projectFilepath = openProjectFileDialog();
		
		if (projectFilepath.isEmpty())
			return std::nullopt;

		if (!FilesystemHandler::isValidFilePath(projectFilepath))
		{
			messageBox("Project Data", "Invalid filepath provided");
			return std::nullopt;
		}
		
		ProjectData projectData(projectFilepath);

		auto& settings = SettingsHandler::settings;
		settings->setValue("project/directory", FilesystemHandler::getFileDirectory(projectFilepath));

		return projectData;
	}

	inline std::optional<ProjectData> createProjectFromDialog()
	{
		CreateProjectDialog projectDialog;
		if (projectDialog.exec() != QDialog::Accepted)
			return std::nullopt;

		auto projectData = projectDialog.getProjectData();
		auto projectFilepath = projectDialog.getFilepath();

		projectData.save(projectFilepath);

		auto& settings = SettingsHandler::settings;
		settings->setValue("project/directory", FilesystemHandler::getFileDirectory(projectFilepath));

		return projectData;
	}
}