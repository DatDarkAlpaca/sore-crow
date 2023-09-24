#pragma once
#include <QFileDialog>

namespace sore
{
	inline QString openProjectFolderDialog()
	{
		return QFileDialog::getExistingDirectory(nullptr, "Open project folder");
	}

	inline QString openProjectFileDialog()
	{
		return QFileDialog::getOpenFileName(
			nullptr,
			"Open project file", "",
			"All files (*.*);;Project files (*.prj)"
		);
	}

	inline QString openSubtitleTrackDialog()
	{
		return QFileDialog::getOpenFileName(
			nullptr,
			"Open project file", "",
			"All files (*.*);;Subtitle files (*.srt)"
		);
	}
}