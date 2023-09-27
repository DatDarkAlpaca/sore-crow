#pragma once
#include <vector>
#include <QString>
#include <QStringList>
#include "srt_components.h"
#include "utils/string_utils.h"

namespace sore
{
	inline srt::Subtitles parseSRT(QTextStream& textStream)
	{
		std::vector<srt::Subtitle> subtitles;

		while (!textStream.atEnd())
		{
			QString line = textStream.readLine().trimmed();

			if (line.isEmpty())
				continue;

			// ID:
			int number = line.toInt();

			// Time Range:
			QString timeRange = textStream.readLine();
			QStringList timeParts = timeRange.split(" --> ");
			QString startTime = timeParts[0];
			QString endTime = timeParts[1];

			double startTimeMs = timeToMilliseconds(startTime);
			double endTimeMs = timeToMilliseconds(endTime);

			// Text:
			QString subtitleText;
			while (!textStream.atEnd())
			{
				QString textLine = textStream.readLine();
				if (textLine.trimmed().isEmpty())
					break;

				subtitleText += textLine + "\n";
			}

			srt::Subtitle subtitle;
			subtitle.text = subtitleText;
			subtitle.startTimeMs = startTimeMs;
			subtitle.endTimeMs = endTimeMs;

			subtitles.push_back(subtitle);
		}

		srt::Subtitles subtitlesSRT;
		subtitlesSRT.subtitles = subtitles;

		return subtitlesSRT;
	}
}