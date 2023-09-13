#pragma once
#include <vector>
#include <string>
#include <filesystem>

namespace sore
{
	inline void createEpisodesFolder(const std::filesystem::path& projectFolderPath, const std::string& folderName)
	{
		namespace fs = std::filesystem;
		fs::create_directory(projectFolderPath / folderName);
	}

	inline std::vector<std::string> moveToEpisodesFolder(const std::vector<std::string>& episodePaths, const std::filesystem::path& folderPath)
	{
		namespace fs = std::filesystem;

		std::vector<std::string> newPaths;

		for (const auto& path : episodePaths)
		{
			fs::path previousFilepath(path);
			fs::path newFilepath(folderPath / previousFilepath.filename());

			fs::rename(previousFilepath, newFilepath);
			newPaths.push_back(newFilepath.string());
		}

		return newPaths;
	}
}