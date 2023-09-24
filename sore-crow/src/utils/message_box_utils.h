#pragma once
#include <string>
#include <QIcon>
#include <QMessageBox>

namespace sore
{
	inline void errorBox(const QString& title, const QString& errorMessage)
	{
		QMessageBox msgBox;
		msgBox.setWindowIcon(QIcon(":/SoreCrow/sore-crow.ico"));

		QString messageBoxTitle = "Sore Crow";

		if (!title.isEmpty())
			messageBoxTitle += " - " + title;
			
		msgBox.setWindowTitle(messageBoxTitle);
		msgBox.setDefaultButton(QMessageBox::Ok);
		msgBox.setText(errorMessage);
		msgBox.exec();
	}
}