#pragma once
#include <QtWidgets/QApplication>

namespace sore
{
	class CrowApp : public QApplication
	{
	public:
		CrowApp(int argc, char** argv);
	};
}