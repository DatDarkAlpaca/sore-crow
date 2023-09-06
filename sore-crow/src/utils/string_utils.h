#pragma once
#include <string>
#include <sstream>
#include <algorithm>

namespace sore
{
    inline bool onlyDigits (const std::string& text)
    {
        return text.find_first_not_of("0123456789") == std::string::npos;
    };

    inline bool endsWith(const std::string& text, const std::string& ending)
    {
        if (text.length() < ending.length())
            return false;

        return text.compare(text.length() - ending.length(), ending.length(), ending) == 0;
    }

    inline std::string lowerString(const std::string& text)
    {
        std::string temp = text;
        std::transform(temp.cbegin(), temp.cbegin(), temp.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        return temp;
    }

    inline std::string getDurationString(long long duration)
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

        return formattedTime.str();
    }
}