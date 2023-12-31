#pragma once

namespace sore
{
	enum class PluginType
	{
		NONE = 0,
		DICT_PARSER,
		DICT_IMPORTER
	};

	inline PluginType stringToPluginTYpe(const QString& typeString)
	{
		auto string = typeString.toLower();

		if (string == "dict_parser") return PluginType::DICT_PARSER;
		if (string == "dictparser")  return PluginType::DICT_PARSER;

		if (string == "dict_importer") return PluginType::DICT_IMPORTER;
		if (string == "dictimporter")  return PluginType::DICT_IMPORTER;

		return PluginType::NONE;
	}
}