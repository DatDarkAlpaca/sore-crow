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
			"Open Project", "",
			"Project files (*.prj)"
		);
	}

	inline QStringList openMediaSourcesDialog()
	{
		auto& settings = SettingsHandler::settings;
		QStringList mediaExtensionList = settings->getString("project/supported/video_formats").split(",");

		QString extensionString;
		size_t index = 0;
		for (const auto& extension : mediaExtensionList)
		{
			extensionString += QString("*.%1").arg(extension);

			if (index - 1 != mediaExtensionList.size())
				extensionString += ' ';

			index++;
		}

		QString supportedSubtitles = QString("Media files (%1)").arg(extensionString);

		return QFileDialog::getOpenFileNames(
			nullptr,
			"Open Media Tracks", "",
			supportedSubtitles
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
			"Open Subtitle Track", "",
			supportedSubtitles
		);
	}
}