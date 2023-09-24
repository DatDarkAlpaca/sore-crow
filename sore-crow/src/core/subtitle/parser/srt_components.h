#pragma once
#include <QString>
#include "core/subtitle/subtitles.h"

namespace sore::srt
{
	struct Subtitle
	{
		QString text;
		double startTimeMs = 0.0, endTimeMs = 0.0;
	};

	struct Subtitles : public ISubtitles
	{
	public:
		Subtitles()
			: ISubtitles(SubtitleType::SRT)
		{

		}

	public:
		std::vector<Subtitle> subtitles;
	};
}