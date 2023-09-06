#pragma once
#include <string>
#include "srt/srt_parser.h"
#include "utils/extension_utils.h"

namespace sore
{
	class SubtitleParser
	{
	public:
		void addParser(IParser* parser);

	public:
		std::vector<SubtitleEntry> parseSubtitleFile(const std::string& filepath);

	public:
		std::vector<IParser*> m_Parsers;
	};
}