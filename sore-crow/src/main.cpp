#include "pch.h"
#include "app/crow_app.h"
#include "gui/view/splash_window/splash_window.h"
#include "gui/view/crow_window/crow_window.h"

int main(int argc, char *argv[])
{
    using namespace sore;

    CrowApp application(argc, argv);

    // Instances:
    SplashWindow* m_SplashWindow = new SplashWindow;
    CrowWindow* m_CrowWindow = new CrowWindow;

    // Connects:
    QObject::connect(m_SplashWindow, &SplashWindow::projectOpened, m_CrowWindow, &CrowWindow::loadProjectData);
    QObject::connect(m_SplashWindow, &SplashWindow::projectCreated, m_CrowWindow, &CrowWindow::loadProjectData);

    m_SplashWindow->show();
    return application.exec();
}