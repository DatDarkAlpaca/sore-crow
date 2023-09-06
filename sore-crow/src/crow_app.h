#pragma once
#include <QtWidgets/QApplication>
#include <QtAdvancedStylesheet.h>

namespace sore
{
	class CrowApp : public QApplication
	{
	public:
		CrowApp(int argc, char** argv);

	private:
		void configureStylesheets();

	private:
		acss::QtAdvancedStylesheet* m_Stylesheet;
	};
}