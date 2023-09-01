#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_splash_window.h"

namespace sore
{
    class SplashWindow : public QMainWindow
    {
        friend class MainController;
        Q_OBJECT

    public:
        SplashWindow(QWidget* parent = nullptr);

        ~SplashWindow() = default;

    private:
        void setupStatusBar();

    private:
        QLabel* m_StatusBarLabel = nullptr;
        Ui::SplashWindow ui;
    };
}