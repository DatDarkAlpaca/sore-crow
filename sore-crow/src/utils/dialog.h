#pragma once
#include <QFileDialog>

namespace sore
{
	inline std::string openProjectFolderDialog()
	{
		return QFileDialog::getExistingDirectory(nullptr, "Open project folder").toStdString();
	}

	inline std::string openProjectFileDialog()
	{
		return QFileDialog::getOpenFileName(
			nullptr,
			"Open project file", "",
			"All files (*.*);;Project files (*.prj)"
		).toStdString();
	}

	inline std::string openSubtitleTrackDialog()
	{
		return QFileDialog::getOpenFileName(
			nullptr,
			"Open project file", "",
			"All files (*.*);;Subtitle files (*.srt)"
		).toStdString();
	}
}