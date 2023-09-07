#pragma once
#include <memory>
#include "subtitle_parser.h"
#include "utils/string_utils.h"

#include "concrete_parsers/srt_parser.h"

namespace sore
{
	class SubtitleFactory
	{
	public:
		static std::unique_ptr<ISubtitleParser> getParser(const std::string& subtitleFilepath)
		{
			std::string lowerFilepath = lowerString(subtitleFilepath);
			if (endsWith(lowerFilepath, "srt"))
				return std::make_unique<SRTParser>(subtitleFilepath);
		}
	};
}