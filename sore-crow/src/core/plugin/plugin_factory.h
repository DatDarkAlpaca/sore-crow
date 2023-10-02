#pragma once
#include "plugin_type.h"
#include "concrete/dictionary_plugin.h"

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

				case PluginType::DICT_IMPORTER:
				{
					auto plugin = std::make_unique<DictionaryPlugin>(manifest);
					return plugin;
				} break;
			}

			return nullptr;
		}
	};
}