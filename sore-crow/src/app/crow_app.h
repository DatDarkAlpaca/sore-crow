#pragma once
#include <QtWidgets/QApplication>
#include "core/plugin/plugin_handler.h"

namespace sore
{
	class CrowApp : public QApplication
	{
	public:
		explicit CrowApp(int argc, char** argv);

	private:
		void setupAttributes();

		void setupHandlers();

	private:
		PluginHandler* m_PluginHandler = nullptr;
	};

	 void preInitializeApp();
}