#pragma once
#include <vector>
#include <memory>
#include "plugin.h"
#include "core/handlers.h"
#include "plugin_factory.h"
#include "utils/message_box_utils.h"

namespace sore
{
	class PluginHandler
	{
	public:
		PluginHandler()
		{
			QString pluginsFolder = SettingsHandler::settings->getString("paths/plugin_path");

			for (const auto& pluginFolder : FilesystemHandler::getFoldersInDirectory(pluginsFolder))
			{
				if(loadPlugin(pluginFolder))
					addPluginDependencyPath();
			}
		}

	public:
		IPlugin* getPlugin(size_t index)
		{
			return m_Plugins[index].get();
		}

	private:
		bool loadPlugin(const QString& pluginFolderPath)
		{
			// Open Manifest:
			QString manifestFilepath = QDir::cleanPath(pluginFolderPath + QDir::separator() + "manifest.json");
			QFile manifest(manifestFilepath);

			if (!manifest.exists())
			{
				QFileInfo fileInfo(pluginFolderPath);
				QString folderName = fileInfo.fileName();

				messageBox("Plugin Handler",
					QString("Plugin %1 has no valid manifest file").arg(folderName));
				return false;
			}

			// Create plugin and manifest:
			Manifest manifestFile(manifestFilepath);
			try {
				manifestFile.initialize();
				auto plugin = m_Factory.createPlugin(manifestFile);

				if (plugin)
				{
					m_Plugins.push_back(std::move(plugin));
					return true;
				}

				messageBox("Plugin Handler", "Failed to import plugin");
				return false;
			}
			catch (const std::exception& e) {
				messageBox("Plugin Handler", e.what());
				return false;
			}

			return true;
		}

		void addPluginDependencyPath()
		{
			IPlugin& addedPlugin = *m_Plugins.back().get();
		
			for (const auto& dependency : addedPlugin.manifest.sources.dependencies)
			{
				QFileInfo fileInfo(dependency);
				
				QLibrary library(fileInfo.absoluteFilePath());
				library.load();
			}
		}

	private:
		std::vector<std::unique_ptr<IPlugin>> m_Plugins;
		PluginFactory m_Factory;
	};
}