#pragma once
#include "ui_preferences.h"

namespace sore
{
	class PreferencesDialog : public QDialog
	{
		Q_OBJECT

	public:
		PreferencesDialog(QWidget* parent = nullptr)
			: QDialog(parent)
		{
			ui.setupUi(this);
			ui.preferencesList->setCurrentRow(0);
		}

	private:
		Ui::PreferencesDialog ui;
	};
}