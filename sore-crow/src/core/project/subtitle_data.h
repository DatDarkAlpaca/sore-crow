#pragma once
#include <string>

namespace sore
{
	struct SubtitleData
	{
		double startSeconds, startMiliseconds; // TODO: change to timedelta or something
		double endSeconds, endMiliseconds;
		std::string text;
	};

	SubtitleData getSubtitleData(const std::string& filepath)
	{
		// TODO: Parse subtitle.
	}
}