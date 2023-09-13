#pragma once
#include <string>
#include <QSettings>
#include "string_utils.h"
#include "crow_settings.h"

namespace sore
{
	inline bool isFileSupportedVideoFormat(const std::string& filepath)
	{
		std::string path = lowerString(filepath);

		auto supportedFormatsString = settings->value("project/supported/video_formats").toString().toStdString();

		for (const auto& extension : split(supportedFormatsString, ","))
		{
			if (endsWith(path, extension))
				return true;
		}

		return false;
	}
}