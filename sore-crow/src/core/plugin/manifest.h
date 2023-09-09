#pragma once
#include <string>
#include "plugin_type.h"

namespace sore
{
	struct Manifest
	{
		std::string pluginName = "", mainFile = "";
		PluginType pluginType = PluginType::UNKNOWN;
	};
}