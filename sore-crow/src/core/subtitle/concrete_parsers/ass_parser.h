#pragma once
#include <string>
#include <array>
#include <regex>
#include <sstream>
#include <unordered_map>

#include "utils/string_utils.h"

#include "core/subtitle/subtitle_data.h"
#include "core/subtitle/subtitle_parser.h"

namespace sore
{
	class ASSParser : public ISubtitleParser
	{
		enum FormatField { MARKED = 0, LAYER, START, END, STYLE, NAME, MARGINL, MARGINR, MARGINV, EFFECT, TEXT, AMOUNT };

	public:
		ASSParser(const std::string& filepath)
			: ISubtitleParser(filepath)
		{
		}

	public:
		std::vector<SubtitleData> parse() override
		{
			std::vector<SubtitleData> results;

			std::string line;

			bool onEventsSection = false;
			std::istringstream stream(fileContents.c_str());
			while (std::getline(stream, line)) 
			{
				// UTF-BOM:
				if (line[0] == (char)0xEF && line[1] == (char)0xBB && line[2] == (char)0xBF)
					line.erase(0, 3);

				if (lowerString(line) == "[Events]")
				{
					onEventsSection = true;
					continue;
				}

				if (!onEventsSection)
					continue;

				// Populate the field table:
				if (startsWith(line, "Format:"))
				{
					line = line.substr(8, line.length() - 1);
					parseFormat(line);
					continue;
				}

				results.push_back(parseEvent(line));
			}

			return results;
		}

	private:
		void parseFormat(const std::string& text)
		{
			auto fields = split(text, ",");

			for (size_t i = 0; i < fields.size(); ++i)
			{
				try {
					auto fieldFound = destroyWhitespace(fields[i]);
					FormatField field = m_FieldTable.at(fieldFound);
					m_FieldOrder[i] = field;
				}
				catch (std::out_of_range) {
					// TODO: handle invalid field found.
					return;
				}
			}
		}

		SubtitleData parseEvent(const std::string& eventText)
		{
			SubtitleData subtitle;

			// Removes "Dialogue:"
			std::string text = eventText.substr(11, eventText.length() - 1);

			auto eventFields = split(text, ",");

			for (size_t i = 0; i < m_FieldOrder.size(); ++i)
			{
				if (i > eventFields.size() - 1)
					break;

				handleCurrentField(eventFields[i], m_FieldOrder[i], subtitle);
			}

			return subtitle;
		}

		void handleCurrentField(const std::string& fieldText, FormatField field, SubtitleData& subtitle)
		{
			switch (field)
			{
				case LAYER:
				case MARKED:
				case STYLE:
				case NAME:
				case MARGINL:
				case MARGINR:
				case MARGINV:
				case EFFECT:
					break;

				case START:
				{
					subtitle.startTimeMilliseconds = parseTimestamp(fieldText);
				} break;

				case END:
				{
					subtitle.endTimeMilliseconds = parseTimestamp(fieldText);
				} break;

				case TEXT:
				{
					subtitle.text = parseText(fieldText);
				} break;
			}
		}

	private:
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

		static std::string parseText(const std::string& text)
		{
			std::regex pattern("\\{[^\\}]*\\}");
			return std::regex_replace(text, pattern, "");
		}
		
	private:
		static inline std::unordered_map<std::string, FormatField> m_FieldTable = {
			{ "Marked",  FormatField::MARKED  },
			{ "Layer",   FormatField::LAYER  },
			{ "Start",	 FormatField::START   },
			{ "End",	 FormatField::END     },
			{ "Style",	 FormatField::STYLE   },
			{ "Name",	 FormatField::NAME    },
			{ "MarginL", FormatField::MARGINL },
			{ "MarginR", FormatField::MARGINR },
			{ "MarginV", FormatField::MARGINV },
			{ "Effect",  FormatField::EFFECT  },
			{ "Text",    FormatField::TEXT    }
		};

		std::array<FormatField, FormatField::AMOUNT> m_FieldOrder = {};
	};
}