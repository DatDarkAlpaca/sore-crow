#pragma once
#include "plugin_type.h"
#include "manifest.h"

namespace sore
{
	class IPlugin
	{
	public:
		IPlugin(PluginType type, const Manifest& manifest)
			: type(type)
			, manifest(manifest) 
		{
			setupLibrary();
		}

		virtual ~IPlugin() = default;

		IPlugin() = default;

	public:
		virtual void execute() = 0;

	private:
		void setupLibrary()
		{
			QFileInfo fileInfo(manifest.sources.mainSource);
			QString mainFile = fileInfo.absoluteFilePath();

			library.setFileName(mainFile);
			library.setLoadHints(QLibrary::ResolveAllSymbolsHint);
			library.load();
		}

	public:
		PluginType type = PluginType::NONE;
		Manifest manifest = {};

	protected:
		QLibrary library;
	};
}