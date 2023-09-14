#pragma once
#include <string>

namespace sore 
{
	struct SubtitleData
	{
		uint64_t startTimeMilliseconds = 0;
		uint64_t endTimeMilliseconds = 0;
		std::string text;
	};
}