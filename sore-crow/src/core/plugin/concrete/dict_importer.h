#pragma once
#include "core/plugin/plugin.h"

namespace sore
{
	using DictImportFunction = void (*) (const char*);

	class DictionaryImporterPlugin : public IPlugin
	{
	public:
		DictionaryImporterPlugin(const Manifest& manifest)
			: IPlugin(PluginType::DICT_IMPORTER, manifest)
		{
		}

		DictionaryImporterPlugin() = default;

	public:
		void setFilepath(const QString& filepath)
		{
			m_CurrentDictionaryFilepath = filepath;
		}

	public:
		void execute() override
		{
			DictImportFunction importFunction = (DictImportFunction)library.resolve(ImportFunctionName);
			
			if (importFunction)
				importFunction(m_CurrentDictionaryFilepath.toStdString().c_str());
			else
				throw std::exception("Could not execute plugin import operation");
		}

	private:
		constexpr static inline const char* ImportFunctionName = "import";
		QString m_CurrentDictionaryFilepath;
	};
}