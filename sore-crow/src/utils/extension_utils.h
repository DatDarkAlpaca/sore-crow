#pragma once
#include <string>
#include "string_utils.h"
#include "core/global_data.h"

namespace sore
{
	inline bool isFileSupportedVideoFormat(const std::string& filepath)
	{
		std::string path = lowerString(filepath);

		for (const auto extension : Macros::supportedVideoFormats)
		{
			if (endsWith(path, extension))
				return true;
		}

		return false;
	}
}