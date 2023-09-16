#include "pch.h"
#include "splash_window.h"

static constexpr const char* GithubURL = "https://github.com/DatDarkAlpaca";
static constexpr const char* StyleText = "color: white;";

namespace sore
{
    SplashWindow::SplashWindow(QWidget* parent)
        : QMainWindow(parent)
    {
        ui.setupUi(this);

        setWindowIcon(QIcon(":/common/crow_icon/crow.ico"));

        // Status Bar:
        setupStatusBar();

        // Header:
        ui.crowLabel->setStyleSheet("font-family: Krub; font-size: 48px;");
    }

    void SplashWindow::setupStatusBar()
    {
        m_StatusBarLabel = new QLabel(this);
        m_StatusBarLabel->setOpenExternalLinks(true);

        QString text = QString("Made by <a href=\"%1\" style=\"%2\">DatAlpaca</a>").arg(GithubURL, StyleText);
        m_StatusBarLabel->setText(text);
        statusBar()->addPermanentWidget(m_StatusBarLabel);
    }
}