#pragma once
#include <string>
#include <numeric>
#include <optional>

namespace sore
{
	struct Timestamp
	{
		uint64_t hours = 0, minutes = 0, seconds = 0, milliseconds = 0;
	};

	struct SubtitleEntry
	{
		std::string text;
		Timestamp start, end;
		uint64_t counter;
	};
}