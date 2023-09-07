#pragma once
#include <vector>
#include "subtitle_data.h"

namespace sore
{
	class ISubtitleParser
	{
	public:
		ISubtitleParser(const std::string& filepath)
		{
			std::ifstream file(filepath);
			if (!file.good())
				throw "Invalid file: " + filepath;

			std::stringstream ss;
			ss << file.rdbuf();

			fileContents = ss.str();
		}

		virtual ~ISubtitleParser() = default;

	public:
		virtual std::vector<SubtitleData> parse() = 0;

	protected:
		std::string fileContents;
	};
}