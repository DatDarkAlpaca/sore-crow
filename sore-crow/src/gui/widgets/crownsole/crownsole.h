#pragma once
#include <QWidget>
#include "ui_crownsole.h"
#include "core/logger/severity.h"
#include "gui/data/global_data.h"

namespace sore
{
	class Crownsole : public QWidget
	{
		Q_OBJECT

	public:
		Crownsole(QWidget* parent = nullptr)
			: QWidget(parent)
		{
			ui.setupUi(this);
		}

	public:
		void log(const std::string& message, Severity severity)
		{
			std::string bracketsColor = "white";
			if (!globalData.isStylesheetDark)
				bracketsColor = "#232629";

			std::string severityString = "INFO";
			std::string severityColor = "#357ABD";

			if (severity == Severity::WARN)
			{
				severityString = "WARN";
				severityColor = "#FFD700";
			}
			else if (severity == Severity::ERROR)
			{
				severityString = "ERROR";
				severityColor = "#E74C3C";
			}

			QString logEntry = QString("<font color=\"%1\">[ </font><font color = \"%2\">%3</font><font color = \"%1\"> ]: </font><font color = \"%2\">%4</font>\n")

				.arg(bracketsColor.c_str())
				.arg(severityColor.c_str())
				.arg(severityString.c_str())
				.arg(message.c_str());

			ui.output->append(logEntry);
		}

	private:
		Ui::Crownsole ui;
	};
}