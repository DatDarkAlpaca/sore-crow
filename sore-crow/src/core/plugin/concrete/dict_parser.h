#pragma once
#include <iostream> //todo:remove
#include "core/plugin/plugin.h"

namespace sore
{
	using DictParserFunction = bool (*) (const char*);
	using DictSetFunction = void (*) (const char*);

	class DictionaryParserPlugin : public IPlugin
	{
	public:
		DictionaryParserPlugin(const Manifest& manifest)
			: IPlugin(PluginType::DICT_PARSER, manifest)
		{
		}

		DictionaryParserPlugin() = default;

	public:
		void setFilepath(const QString& filepath)
		{
			m_CurrentDictionaryFilepath = filepath;
		}

		void setOutputFolder(const QString& folderPath)
		{
			m_OutputFolder = folderPath;
		}

	public:
		void execute() override
		{
			DictSetFunction setOutput = (DictSetFunction)library.resolve(SetOutputFolderFunctionName);
			if (setOutput)
				setOutput(m_OutputFolder.toStdString().c_str());
			else
				throw std::exception("Could not execute plugin setter operation");

			// Parse:
			DictParserFunction parseFunction = (DictParserFunction)library.resolve(ParseFunctionName);

			if (parseFunction)
				parseFunction(m_CurrentDictionaryFilepath.toStdString().c_str());
			else
				throw std::exception("Could not execute plugin parsing operation");
		}

	private:
		constexpr static inline const char* ParseFunctionName = "parse";
		constexpr static inline const char* SetOutputFolderFunctionName = "setOutputFolder";
		QString m_CurrentDictionaryFilepath, m_OutputFolder;
	};
}