#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include "utils/string_utils.h"
#include "core/global_data.h"

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
}