#pragma once
#include <string>
#include <fstream>

#include "srt/srt_parser.h"
#include "utils/extension_utils.h"
#include "subtitle_parser.h"

namespace sore
{
	void SubtitleParser::addParser(IParser* parser)
	{
		m_Parsers.push_back(parser);
	}

	std::vector<SubtitleEntry> SubtitleParser::parseSubtitleFile(const std::string& filepath)
	{
		if (!isFileSupportedSubtitle(filepath))
			return;

		std::ifstream file(filepath);

		std::stringstream ss;
		ss << file.rdbuf();

		for (const auto& parser : m_Parsers)
		{
			if (parser->isValidFile(filepath))
				return parser->parse(ss.str());
		}
	}
}