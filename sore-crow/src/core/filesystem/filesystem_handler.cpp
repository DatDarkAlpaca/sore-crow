#include "pch.h"
#include "constants.h"
#include "filesystem_handler.h"
#include "utils/string_utils.h"
#include "core/settings/settings_handler.h"

namespace sore
{
	void FilesystemHandler::initialize()
	{
		std::filesystem::current_path(qApp->applicationDirPath().toStdString());

		createDefaultDirectories();
	}

	std::filesystem::path FilesystemHandler::getAbsolutePath(const QString& entry)
	{
		namespace fs = std::filesystem;
		auto& settings = SettingsHandler::settings;
		return fs::path(qApp->applicationDirPath().toStdString()) / settings->getString(entry).toStdString();
	}

	QString FilesystemHandler::getFileDirectory(const QString& filepath)
	{
		QFileInfo fileInfo(filepath);
		QString path = fileInfo.dir().path() + "/";
		return path;
	}

	std::vector<QString> FilesystemHandler::getFilesInDirectory(const QString& directoryPath)
	{
		QDir directory(directoryPath);

		std::vector<QString> filepaths = {};
		for (const QFileInfo& fileInfo : directory.entryInfoList(QDir::Files))
			filepaths.push_back(fileInfo.filePath());

		return filepaths;
	}

	QJsonObject FilesystemHandler::loadFromJsonFile(const QString& filepath)
	{
		QFile file(filepath);
		file.open(QIODevice::ReadOnly);
		QByteArray fileData = file.readAll();
		file.close();

		QJsonDocument document(QJsonDocument::fromJson(fileData));
		return document.object();
	}

	bool FilesystemHandler::isValidFilePath(const QString& filepath)
	{
		QFileInfo fileInfo(filepath);
		return fileInfo.isFile();
	}

	bool FilesystemHandler::isValidFolderPath(const QString& folderPath)
	{
		QFileInfo fileInfo(folderPath);
		return fileInfo.isDir();
	}

	bool FilesystemHandler::isFileSupportedVideoFormat(const QString& filepath)
	{
		QString path = filepath.toLower();

		auto& settings = SettingsHandler::settings;
		for (const auto& extension : settings->getString("project/supported/video_formats").split(","))
		{
			if (path.endsWith(extension))
				return true;
		}

		return false;
	}

	QString FilesystemHandler::getFilename(const QString& filepath)
	{
		QFileInfo fileInfo(filepath);
		return fileInfo.fileName();
	}

	QString FilesystemHandler::getMediaFilepath(const QString& mediaFilepath)
	{
		auto& settings = SettingsHandler::settings;

		QString filepath = settings->getString("project/directory") + mediaFilepath;
		return filepath;
	}

	void FilesystemHandler::createDefaultDirectories()
	{
		namespace fs = std::filesystem;
		auto& settings = SettingsHandler::settings;

		settings->beginGroup("paths");
		for (const auto& pathKey : settings->childKeys())
		{
			auto path = settings->getString(pathKey);
			if (path.isEmpty())
				continue;

			fs::create_directory(path.toStdString());
		}
		settings->endGroup();
	}
}