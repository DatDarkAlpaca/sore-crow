#pragma once
#include <QtWidgets/QApplication>

namespace sore
{
	class CrowApp : public QApplication
	{
	public:
		explicit CrowApp(int argc, char** argv);

	private:
		void setupAttributes();

		void setupHandlers();
	};
}