#pragma once
#include <string>
#include <vector>

namespace sore
{
    struct CreateProjectDialogData
    {
        std::string projectName;
        std::string projectFilepath;
        std::vector<std::string> episodePaths;
        bool moveEpisodes;
    };
}