#pragma once
#include <QMainWindow>
#include "ui_splash_window.h"
#include "gui/model/project_data.h"

namespace sore
{
	class SplashWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		SplashWindow(QWidget* parent = nullptr);

		~SplashWindow() = default;

	private slots:
		void onOpenProjectButton();

		void onCreateProjectButton();

	signals:
		void projectOpened(const ProjectData&);

		void projectCreated(const ProjectData&);

	private:
		void setupStatusBar();

		void setupStyle();

	private:
		Ui::SplashWindow ui;
		QLabel* m_StatusBarLabel = nullptr;
	};
}