#include "pch.h"
#include "project_data.h"
#include "crow_settings.h"

#include "utils/string_utils.h"
#include "utils/message_box.h"
#include "core/logger/logger.h"

namespace sore
{
    std::optional<ProjectData> getProjectData(const std::string& filepath)
    {
        using namespace nlohmann;

        auto projectExtension = settings->value("project/extension", "prj").toString().toStdString();

        std::ifstream file(filepath);
        if (file.bad())
        {
            CrownsoleLogger::log("Failed to retrieve project data from " + filepath + ".", Severity::ERROR);
            return std::nullopt;
        }

        if (!endsWith(filepath, projectExtension))
        {
            CrownsoleLogger::log("This project file ends in the wrong extension.", Severity::ERROR);
            return std::nullopt;
        }

        try
        {
            ProjectData data(json::parse(file), filepath);
            return data;
        }
        catch (nlohmann::json::exception e) {
            CrownsoleLogger::log(e.what(), Severity::ERROR);
            errorBox(e.what());
            return std::nullopt;
        }
    }

    void createProjectFile(const ProjectData& data, const std::string& projectDirectory)
    {
        namespace fs = std::filesystem;
        using namespace nlohmann;

        auto projectExtension = settings->value("project/extension", "prj").toString().toStdString();
        fs::path projectFilepath = fs::path(projectDirectory) / (data.header.projectName + "." + projectExtension);
        
        auto jsonData = data.toJSON();
        std::ofstream outputFile(projectFilepath);
        outputFile << jsonData.dump(4, 32, false, json::error_handler_t::ignore);
    }
}