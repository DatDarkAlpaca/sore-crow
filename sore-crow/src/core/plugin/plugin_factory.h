#pragma once
#include "plugin_type.h"
#include "concrete/dict_importer.h"
#include "concrete/dict_parser.h"

namespace sore
{
	class PluginFactory
	{
	public:
		std::unique_ptr<IPlugin> createPlugin(const Manifest& manifest)
		{
			switch (manifest.pluginType)
			{
				case PluginType::NONE:
					throw "invalid plugin type";

				case PluginType::DICT_PARSER:
				{
					auto plugin = std::make_unique<DictionaryParserPlugin>(manifest);
					return plugin;
				} break;

				case PluginType::DICT_IMPORTER:
				{
					auto plugin = std::make_unique<DictionaryImporterPlugin>(manifest);
					return plugin;
				} break;
			}

			return nullptr;
		}
	};
}