#pragma once
#include <string>
#include <algorithm>

namespace sore
{
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
}