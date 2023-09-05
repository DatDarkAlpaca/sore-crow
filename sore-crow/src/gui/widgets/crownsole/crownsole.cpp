#include "pch.h"
#include "crownsole.h"

namespace sore
{
	Crownsole::Crownsole(QWidget* parent)
	{
		ui.setupUi(this);
	}

	void Crownsole::log(const std::string& message, Severity severity)
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
}
