#pragma once
#include <string>
#include <Python.h>
#include "base_plugin.h"

namespace sore
{
	class DictionaryPlugin : public BasePlugin
	{
	public:
		DictionaryPlugin(const std::string& pluginFolder)
			: BasePlugin(pluginFolder)
		{
		}

	public:
		void setDictionaryPath(const std::string& path)
		{
			m_DictionaryPath = path;
		}

		void handleCommand(PyObject* module) override
		{
			PyObject* pFunc = PyObject_GetAttrString(module, "parse");
			if (pFunc && PyCallable_Check(pFunc))
			{
				PyObject* result = PyObject_CallFunction(pFunc, "s", m_DictionaryPath.c_str());

				PyObject* pResultStr = PyObject_Repr(result);
				std::string returnedString = PyUnicode_AsUTF8(pResultStr);
				//std::cout << returnedString << std::endl;
			}
		}

	private:
		std::string m_DictionaryPath;
	};
}