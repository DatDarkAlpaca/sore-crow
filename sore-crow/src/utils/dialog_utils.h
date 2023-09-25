#pragma once
#include <QFileDialog>
#include "core/handlers.h"

namespace sore
{
	inline QString openProjectFolderDialog()
	{
		return QFileDialog::getExistingDirectory(nullptr, "Open Project folder");
	}

	inline QString openStylesFolderDialog()
	{
		return QFileDialog::getExistingDirectory(nullptr, "Open Styles folder");
	}

	inline QString openProjectFileDialog()
	{
		return QFileDialog::getOpenFileName(
			nullptr,
			"Open project file", "",
			"Project files (*.prj)"
		);
	}

	inline QString openSubtitleTrackDialog()
	{
		auto& settings = SettingsHandler::settings;
		QStringList subExtensionList = settings->getString("project/supported/subtitle_formats").split(",");
		
		QString extensionString;
		size_t index = 0;
		for (const auto& extension : subExtensionList)
		{
			extensionString += QString("*.%1").arg(extension);

			if (index - 1 != subExtensionList.size())
				extensionString += ' ';

			index++;
		}

		QString supportedSubtitles = QString("Subtitle files (%1)").arg(extensionString);

		return QFileDialog::getOpenFileName(
			nullptr,
			"Open project file", "",
			supportedSubtitles
		);
	}
}