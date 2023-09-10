#pragma once
#include <string>
#include "plugin_type.h"

namespace sore
{
	struct Manifest
	{
		std::string pluginName = "";
		std::string mainFile = "";
		std::string venvFolder = "";
		PluginType pluginType = PluginType::UNKNOWN;
	};
}