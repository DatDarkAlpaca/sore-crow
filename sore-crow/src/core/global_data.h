#pragma once
#include <QApplication>
#include <string>

namespace sore
{
	static inline std::string ResourcesPath;
	static inline std::string StylePath;
	static inline std::string StyleOutputPath;

	struct Data
	{
	public:
		static void initialize()
		{
			ResourcesPath = qApp->applicationDirPath().toStdString() + "/res";
			StylePath = qApp->applicationDirPath().toStdString() + "/res/styles";
			StyleOutputPath = qApp->applicationDirPath().toStdString() + "/res/output";
		}

	public:
		bool isStylesheetDark = true;
	} inline globalData;
}