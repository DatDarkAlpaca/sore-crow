#pragma once
#include <string>
#include <vector>

namespace sore
{
    struct CreateProjectDialogData
    {
        std::string projectName;
        std::string projectRootFolder;
        std::vector<std::string> episodePaths;
    };
}