#pragma once
#include <vector>
#include <QString>
#include <QStringList>

#include "sbv_components.h"
#include "utils/string_utils.h"

namespace sore
{
	inline sbv::Subtitles parseSBV(QTextStream& textStream)
	{
		std::vector<sbv::Subtitle> subtitles;

		while (!textStream.atEnd())
		{
			QString line = textStream.readLine().trimmed();

			if (line.isEmpty())
				continue;

			// Time Range:
			QStringList timeParts = line.split(",");
			QString startTime = timeParts[0];
			QString endTime = timeParts[1];

			double startTimeMs = timeToMilliseconds(startTime, QRegularExpression("[:.]"));
			double endTimeMs = timeToMilliseconds(endTime, QRegularExpression("[:.]"));

			// Text:
			QString subtitleText;
			while (!textStream.atEnd())
			{
				QString textLine = textStream.readLine();
				if (textLine.trimmed().isEmpty())
					break;

				subtitleText += textLine + "\n";
			}

			sbv::Subtitle subtitle;
			subtitle.text = subtitleText;
			subtitle.startTimeMs = startTimeMs;
			subtitle.endTimeMs = endTimeMs;

			subtitles.push_back(subtitle);
		}

		sbv::Subtitles subtitlesSBV;
		subtitlesSBV.subtitles = subtitles;

		return subtitlesSBV;
	}
}