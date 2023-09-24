#include "pch.h"
#include "constants.h"
#include "splash_window.h"
#include "core/handlers.h"
#include "gui/model/project_utils.h"

namespace sore
{
	SplashWindow::SplashWindow(QWidget* parent)
		: QMainWindow(parent)
	{
		ui.setupUi(this);
		setWindowTitle(windowTitle() + " v" + ProjectVersion);

		setupStatusBar();
		
		setupStyle();

		connect(ui.openProjectBtn, &QPushButton::released, this, &SplashWindow::onOpenProjectButton);
		connect(ui.createProjectBtn, &QPushButton::released, this, &SplashWindow::onCreateProjectButton);
	}

	void SplashWindow::onOpenProjectButton()
	{
		auto projectData = openProjectFromDialog();
		if (!projectData.has_value())
			return;

		emit projectOpened(projectData.value());
		hide();
	}

	void SplashWindow::onCreateProjectButton()
	{
		auto projectData = createProjectFromDialog();
		if (!projectData.has_value())
			return;

		emit projectCreated(projectData.value());
		hide();
	}

	void SplashWindow::setupStatusBar()
	{
		auto& stylesheet = StylesheetHandler::stylesheet;

		m_StatusBarLabel = new QLabel(this);
		m_StatusBarLabel->setOpenExternalLinks(true);

		QString style = "color: black";
		if(stylesheet->isCurrentThemeDark())
			style = "color: white";

		QString text = QString("Made by <a href=\"%1\" style=\"%2\">DatAlpaca</a>")
			.arg(links::GithubURL, style);

		m_StatusBarLabel->setText(text);
		statusBar()->addPermanentWidget(m_StatusBarLabel);
	}

	void SplashWindow::setupStyle()
	{
		ui.crowLabel->setStyleSheet("font-size: 48px;");
	}
}