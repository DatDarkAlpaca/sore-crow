#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include "utils/string_utils.h"
#include "core/macros/macros.h"

namespace sore
{
	inline std::vector<std::string> getFilesInDir(const std::string& dirpath)
	{
		namespace fs = std::filesystem;

		std::vector<std::string> filepaths = {};
		for (const auto& entry : fs::recursive_directory_iterator(dirpath))
		{
			if(entry.is_regular_file())
				filepaths.push_back(entry.path().string());
		}

		return filepaths;
	}

	inline bool isValidEpisodesDirectory(const std::string& directory)
	{
		for (const auto& supportedEpisodeFolderName : Macros::AcceptableEpisodeFolderNames)
		{
			if (endsWith(lowerString(directory), supportedEpisodeFolderName))
				return true;
		}

		return false;
	}
}