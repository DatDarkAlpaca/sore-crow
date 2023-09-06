#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <optional>
#include <iostream>

#include "core/parser/objects.h"
#include "utils/string_utils.h"
#include "core/parser/parser.h"

namespace sore
{
	class SRTParser : public IParser
	{
	public:
		std::vector<SubtitleEntry> parse(const std::string& fileContents) override;

		bool isValidFile(const std::string& filepath) const override;
	};
}