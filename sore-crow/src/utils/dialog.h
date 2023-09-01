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
}