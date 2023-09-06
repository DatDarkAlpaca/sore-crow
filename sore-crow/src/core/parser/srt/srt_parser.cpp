#include "pch.h"
#include "srt_parser.h"
#include "utils/string_utils.h"

static std::optional<sore::Timestamp> parseSRTTimestamp(const std::string& timestampText)
{
	sore::Timestamp timestamp;
	std::istringstream ss(timestampText);

	uint64_t hour, minute, second, millisecond;
	char delimiter;
	if (ss >> hour && ss >> delimiter && ss >> minute && ss >> delimiter && ss >> second && ss >> delimiter && ss >> millisecond)
		return sore::Timestamp{ hour, minute, second, millisecond };
	else
		return std::nullopt;
}

static std::vector<sore::Timestamp> getTimestamps(const std::string& content)
{
	std::string delimiter = "-->";
	std::vector<sore::Timestamp> results(2);
	int pos = content.find_first_of(delimiter);

	std::string firstTimestamp = content.substr(0, pos - 1);
	std::string secondTimestamp = content.substr(pos + delimiter.size() + 1);

	auto firstTimestampParsed = parseSRTTimestamp(firstTimestamp);
	auto secondTimestampParsed = parseSRTTimestamp(secondTimestamp);

	if (firstTimestampParsed.has_value())
		results[0] = firstTimestampParsed.value();

	if (secondTimestampParsed.has_value())
		results[1] = secondTimestampParsed.value();

	return results;
}

namespace sore
{
	std::vector<SubtitleEntry> SRTParser::parse(const std::string& fileContents)
	{
		enum class TokenClass { NONE = 0, COUNTER, TIMESTAMP, TEXT };

		std::vector<SubtitleEntry> results;
		SubtitleEntry currentEntry;

		std::string line;
		line.reserve(0);

		bool firstEntry = true;
		TokenClass token = TokenClass::NONE;

		std::istringstream stream(fileContents.c_str());
		while (std::getline(stream, line))
		{
			// UTF-BOM:
			if (line[0] == (char)0xEF && line[1] == (char)0xBB && line[2] == (char)0xBF)
				line.erase(0, 3);

			if (firstEntry)
			{
				if (line.empty())
					continue;
				else
				{
					token = TokenClass::COUNTER;
					firstEntry = false;
				}
			}

			switch (token)
			{
				case TokenClass::COUNTER:
				{
					currentEntry.counter = std::stoi(line);
					token = TokenClass::TIMESTAMP;
				} break;

				case TokenClass::TIMESTAMP:
				{
					std::vector<Timestamp> timestamps = getTimestamps(line);
					currentEntry.start = timestamps[0];
					currentEntry.end = timestamps[1];
					token = TokenClass::TEXT;
				} break;

				case TokenClass::TEXT:
				{
					if (line.empty())
					{
						results.push_back(currentEntry);
						token = TokenClass::COUNTER;
						currentEntry.text = "";
						continue;
					}
					currentEntry.text += line;
				} break;
			}
		}

		return results;
	}

	bool SRTParser::isValidFile(const std::string& filepath) const
	{
		std::string path = lowerString(filepath);
		return (endsWith(path, "srt"));
	}
}