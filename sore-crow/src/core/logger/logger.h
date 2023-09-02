#pragma once
#include "gui/widgets/crownsole/crownsole.h"
#include "severity.h"

namespace sore
{
    class CrownsoleLogger : public QObject
    {
        Q_OBJECT

    public:
        static void setConsoleWidget(Crownsole* widget)
        {
            crownsole = widget;
        }

        static void log(const std::string& message, Severity severity)
        {
            if (!crownsole)
                return;

            crownsole->log(message, severity);
        }

    private:
        static inline Crownsole* crownsole = nullptr;
    };
}