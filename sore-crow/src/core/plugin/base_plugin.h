#pragma once
#include <string>
#include <fstream>
#include <Python.h>
#include <cstdlib>
#include <nlohmann/json.hpp>

#include "manifest.h"
#include "core/global_data.h"
#include "core/logger/logger.h"
#include "utils/string_utils.h"

namespace sore
{
	class BasePlugin
	{
	public:
		BasePlugin(const std::string& pluginFolder)
		{
			m_PluginFolder = std::filesystem::path(PluginPath) / pluginFolder;

			loadManifest();
		}

	public:
		void execute()
		{
			PyObject* mainModule = initializePythonInterpreter();
			if (!mainModule)
				return;

			handleCommand(mainModule);
			Py_DECREF(mainModule);

			shutdownPythonInterpreter();
		}

	protected:
		virtual void handleCommand(PyObject* module) = 0;

	private:
		void loadManifest()
		{
			namespace fs = std::filesystem;
			using namespace nlohmann;

			fs::path manifestPath = fs::path(PluginPath) / m_PluginFolder / "manifest.json";
			if (!fs::is_regular_file(manifestPath))
			{
				CrownsoleLogger::log("Missing manifest.json", Severity::ERROR);
				return;
			}

			std::ifstream file(manifestPath.string());

			try
			{
				json data = json::parse(file);
				m_Manifest.pluginName = data["plugin-name"];
				m_Manifest.pluginType = getPluginType(data["plugin-type"]);
				m_Manifest.venvFolder = data["venv-folder"];
				m_Manifest.mainFile = data["main"];
			}
			catch (json::exception e) {
				CrownsoleLogger::log(e.what(), Severity::ERROR);
				return;
			}
		}

		PyObject* initializePythonInterpreter()
		{
			namespace fs = std::filesystem;
			fs::path venvPath = m_PluginFolder / m_Manifest.venvFolder / "Lib/site-packages";

			// Change CWD:
			fs::current_path(m_PluginFolder);

			// Initialize Python:
			PyConfig config;
			PyConfig_InitPythonConfig(&config);

			PyStatus status = Py_InitializeFromConfig(&config);
			PyConfig_Clear(&config);

			if (PyStatus_Exception(status))
				Py_ExitStatusException(status);

			// Initialize sys.path:
			PyObject* sys = PyImport_ImportModule("sys");
			PyObject* path = PyObject_GetAttrString(sys, "path");
			Py_DECREF(sys);

			PyList_Append(path, PyUnicode_FromString(m_PluginFolder.string().c_str()));
			PyList_Append(path, PyUnicode_FromString(venvPath.string().c_str()));
			Py_DECREF(path);

			// Initialize main module:
			std::string mainFilename = removeExtension(m_Manifest.mainFile);
			PyObject* mainImport = PyUnicode_FromString(mainFilename.c_str());
			PyObject* mainModule = PyImport_Import(mainImport);
			Py_DECREF(mainImport);

			if (!mainModule)
			{
				CrownsoleLogger::log("Failed to initialize: " + mainFilename, Severity::WARN);
				return nullptr;
			}

			if (PyErr_Occurred())
			{
				// TODO: handle.
				CrownsoleLogger::log("An erorr has occurred while running your python script", Severity::WARN);
				return nullptr;
			}

			return mainModule;
		}

		void shutdownPythonInterpreter()
		{
			namespace fs = std::filesystem;

			Py_FinalizeEx();
			fs::current_path(WorkingDirectory);
		}

	public:
		Manifest manifest() const { return m_Manifest; }

	protected:
		std::filesystem::path m_PluginFolder;
		Manifest m_Manifest;
	};
}