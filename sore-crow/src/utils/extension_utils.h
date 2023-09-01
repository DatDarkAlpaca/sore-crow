#pragma once
#include <string>
#include "string_utils.h"
#include "core/macros/macros.h"

namespace sore
{
	inline bool isFileSupportedVideo(const std::string& filepath)
	{
		std::string path = lowerString(filepath);

		for (const auto extension : Macros::supportedVideoFormats)
		{
			if (endsWith(path, extension))
				return true;
		}

		return false;
	}

	inline bool isFileSupportedSubtitle(const std::string& filepath)
	{
		std::string path = lowerString(filepath);

		for (const auto extension : Macros::supportedSubtitleFormats)
		{
			if (endsWith(path, extension))
				return true;
		}

		return false;
	}
}