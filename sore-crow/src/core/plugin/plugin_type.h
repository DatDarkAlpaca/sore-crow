#pragma once
#include <string>

namespace sore
{
	enum class PluginType
	{
		UNKNOWN = 0,
		DICTIONARY_IMPORTER
	};

	PluginType getPluginType(const std::string& pluginType)
	{
		if (pluginType == "DictionaryImporter")
			return PluginType::DICTIONARY_IMPORTER;

		return PluginType::UNKNOWN;
	}
}