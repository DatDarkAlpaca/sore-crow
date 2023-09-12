#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <array>

#include "core/subtitle/subtitle_data.h"
#include "core/subtitle/subtitle_parser.h"

namespace sore
{
	class SRTParser : public ISubtitleParser
	{
	public:
		SRTParser(const std::string& filepath)
			: ISubtitleParser(filepath)
		{

		}

	public:
		std::vector<SubtitleData> parse() override
		{
			enum class TokenClass { NONE = 0, COUNTER, TIMESTAMP, TEXT };
			auto onlyDigits = [](const std::string& text)
			{
				return text.find_first_not_of("0123456789") == std::string::npos;
			};

			std::vector<SubtitleData> results;
			SubtitleData currentEntry;

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
					token = TokenClass::TIMESTAMP;
				} break;

				case TokenClass::TIMESTAMP:
				{
					auto timestamps = getTimestamps(line);
					currentEntry.startTimeMilliseconds = timestamps[0];
					currentEntry.endTimeMilliseconds = timestamps[1];
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

	private:
		static std::array<uint64_t, 2> getTimestamps(const std::string& content)
		{
			std::string delimiter = "-->";
			std::array<uint64_t, 2> results;
			int pos = content.find_first_of(delimiter);

			std::string firstTimestamp = content.substr(0, pos - 1);
			std::string secondTimestamp = content.substr(pos + delimiter.size() + 1);

			results[0] = parseTimestamp(firstTimestamp);
			results[1] = parseTimestamp(secondTimestamp);

			return results;
		}

		static uint64_t parseTimestamp(const std::string& timestampText)
		{
			std::istringstream ss(timestampText);

			uint64_t hour, minute, second, millisecond;
			char delimiter;
			if (ss >> hour && ss >> delimiter && ss >> minute && ss >> delimiter && ss >> second && ss >> delimiter && ss >> millisecond)
				return millisecond + second * 1000 + minute * 60000 + hour * 3.6e+6;

			// TODO: improve:
			else
				throw "Invalid timestamp format: " + timestampText;
		}
	};
}