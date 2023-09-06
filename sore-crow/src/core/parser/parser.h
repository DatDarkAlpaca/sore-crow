#pragma once
#include <string>
#include "objects.h"

namespace sore
{
	class IParser 
	{
	public:
		virtual ~IParser() = default;

	public:
		virtual std::vector<SubtitleEntry> parse(const std::string& fileContents) = 0;

		virtual bool isValidFile(const std::string& filepath) const = 0;
	};
}