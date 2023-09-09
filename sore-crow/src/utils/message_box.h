#pragma once
#include <string>
#include <QMessageBox>

namespace sore
{
	inline void errorBox(const std::string& errorMessage)
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle("Sore Crow");
		msgBox.setDefaultButton(QMessageBox::Ok);
		msgBox.setText(errorMessage.c_str());
		msgBox.exec();
	}
}