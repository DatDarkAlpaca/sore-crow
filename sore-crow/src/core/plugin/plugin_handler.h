#pragma once
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include <python3.10/Python.h>

#include "core/logger/logger.h"
#include "core/global_data.h"
#include "manifest.h"

namespace sore
{
	class IPlugin
	{
	public:
		IPlugin(const std::string& pluginName)
		{
			loadManifest(pluginName);
		}

	private:
		void loadManifest(const std::string& pluginName)
		{
			namespace fs = std::filesystem;
			using namespace nlohmann;

			fs::path manifestPath = fs::path(PluginPath) / pluginName / "manifest.json";
			std::ifstream file(manifestPath.string());

			/*json data = json::parse(file);
			m_Manifest.pluginName = data["plugin-name"];
			m_Manifest.pluginType = getPluginType(data["plugin-type"]);
			m_Manifest.mainFile = data["main"];*/
		}

	private:
		
	};

	class PluginHandler
	{
	public:
		PluginHandler()
		{
			Py_Initialize();
		}

	private:

	};
}