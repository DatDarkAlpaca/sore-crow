#pragma once
#include <filesystem>

namespace sore
{
	class FilesystemHandler
	{
	public:
		static void initialize();

	public:
		static std::filesystem::path getAbsolutePath(const QString& entry);

		static QString getFileDirectory(const QString& filepath);

		static std::vector<QString> getFilesInDirectory(const QString& directoryPath);

	public:
		static QJsonObject loadFromJsonFile(const QString& filepath);

	public:
		static bool isValidFilePath(const QString& filepath);
		
		static bool isValidFolderPath(const QString& folderPath);

		static bool isFileSupportedVideoFormat(const QString& filepath);

	public:
		static QString getFilename(const QString& filepath);

	public:
		static QString getMediaFilepath(const QString& mediaFilepath);

	private:
		static void createDefaultDirectories();
	};
}