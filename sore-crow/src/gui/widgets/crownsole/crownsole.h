#pragma once
#include <QWidget>
#include "ui_crownsole.h"
#include "core/logger/severity.h"

namespace sore
{
	class Crownsole : public QWidget
	{
		Q_OBJECT

	public:
		Crownsole(QWidget* parent = nullptr);

	public:
		void log(const std::string& message, Severity severity);

	private:
		Ui::Crownsole ui;
	};
}