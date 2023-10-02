#pragma once
#include <iostream> //todo:remove
#include "core/plugin/plugin.h"

namespace sore
{
	using DictParseFunction = void (*) (const char*);

	class DictionaryPlugin : public IPlugin
	{
	public:
		DictionaryPlugin(const Manifest& manifest)
			: IPlugin(PluginType::DICT_IMPORTER, manifest)
		{
		}

		DictionaryPlugin() = default;

	public:
		void setFilepath(const QString& filepath)
		{
			m_CurrentDictionaryFilepath = filepath;
		}

	public:
		void execute() override
		{
			QFileInfo fileInfo(manifest.sources.mainSource);
			QString mainFile = fileInfo.absoluteFilePath();

			QLibrary library(mainFile);
			library.setLoadHints(QLibrary::ResolveAllSymbolsHint);

			DictParseFunction parseFunction = (DictParseFunction)library.resolve("parse");
			
			if (parseFunction)
				parseFunction(m_CurrentDictionaryFilepath.toStdString().c_str());
			else
				throw std::exception("Could not execute plugin parsing operation");
		}

	private:
		constexpr static inline const char* ParseFunctionName = "parse";
		QString m_CurrentDictionaryFilepath;
	};
}