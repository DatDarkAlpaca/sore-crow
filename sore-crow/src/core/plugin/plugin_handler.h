#pragma once
#include <string>
#include <fstream>
#include <Python.h>
#include <nlohmann/json.hpp>
#include "manifest.h"
#include "core/global_data.h"
#include "core/logger/logger.h"

namespace sore
{
	class Plugin
	{
	public:
		Plugin(const std::string& pluginFolder)
			: m_PluginFolder(pluginFolder)
		{
			loadManifest();
			loadFile();
		}

	public:
		void execute()
		{
			namespace fs = std::filesystem;
			fs::path mainFilePath = fs::path(PluginPath) / m_PluginFolder / m_Manifest.mainFile;
			PyRun_SimpleFile(m_File, mainFilePath.string().c_str());
		}

	private:
		void loadManifest()
		{
			namespace fs = std::filesystem;
			using namespace nlohmann;

			fs::path manifestPath = fs::path(PluginPath) / m_PluginFolder / "manifest.json";
			std::ifstream file(manifestPath.string());

			json data = json::parse(file);
			m_Manifest.pluginName = data["plugin-name"];
			m_Manifest.pluginType = getPluginType(data["plugin-type"]);
			m_Manifest.mainFile = data["main"];
		}

		void loadFile()
		{
			namespace fs = std::filesystem;
			fs::path mainFilePath = fs::path(PluginPath) / m_PluginFolder / m_Manifest.mainFile;

			m_File = fopen(mainFilePath.string().c_str(), "r");
			if (!m_File)
			{
				CrownsoleLogger::log("Failed to open plugin at: " + mainFilePath.string(), Severity::ERROR);
				fclose(m_File);
				return;
			}
		}

	public:
		Manifest manifest() const { return m_Manifest; }

	private:
		std::string m_PluginFolder;
		Manifest m_Manifest;
		FILE* m_File;
	};
}