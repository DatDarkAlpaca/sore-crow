#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <filesystem>

#include <QString>
#include <QRegularExpression>

namespace sore
{
    inline QString getDurationString(long long duration)
    {
        constexpr long long msPerSecond = 1000;
        constexpr long long msPerMinute = msPerSecond * 60;
        constexpr long long msPerHour = msPerMinute * 60;
        constexpr long long msPerDay = msPerHour * 24;

        long long days = duration / msPerDay;
        duration %= msPerDay;
        long long hours = duration / msPerHour;
        duration %= msPerHour;
        long long minutes = duration / msPerMinute;
        duration %= msPerMinute;
        long long seconds = duration / msPerSecond;

        std::stringstream formattedTime;
        formattedTime << std::setfill('0');

        if (days > 0)
            formattedTime << std::setw(2) << days << ":";

        if (hours > 0 || days > 0)
            formattedTime << std::setw(2) << hours << ":";

        formattedTime << std::setw(2) << minutes << ":";
        formattedTime << std::setw(2) << seconds;

        return formattedTime.str().c_str();
    }

    inline double timeToMilliseconds(const QString& timeString)
    {
        QStringList timeComponents = timeString.split(QRegularExpression("[:,]"));

        double hours = 0, minutes = 0, seconds = 0, milliseconds = 0;
        if (timeComponents.size() == 4)
        {
            hours = timeComponents[0].toDouble();
            minutes = timeComponents[1].toDouble();
            seconds = timeComponents[2].toDouble();
            milliseconds = timeComponents[3].toDouble();
        }
        else if (timeComponents.size() == 3)
        {
            hours = timeComponents[0].toDouble();
            minutes = timeComponents[1].toDouble();
            seconds = timeComponents[2].toDouble();
        }

        return hours * 3600000LL + minutes * 60000LL + seconds * 1000LL + milliseconds;
    }

    inline QString toCamelCase(const QString& text)
    {
        QStringList parts = text.split(' ', Qt::SkipEmptyParts);
        for (int i = 0; i < parts.size(); ++i)
            parts[i].replace(0, 1, parts[i][0].toUpper());

        return parts.join(" ");
    }
}