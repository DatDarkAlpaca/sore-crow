#pragma once
#include <string>

namespace sore 
{
	struct SubtitleData
	{
		uint64_t startTimeMilliseconds;
		uint64_t endTimeMilliseconds;
		std::string text;
	};
}