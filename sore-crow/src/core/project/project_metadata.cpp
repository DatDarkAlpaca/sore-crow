#include "pch.h"
#include "metadata.h"
#include "project_data.h"
#include "utils/uuid_utils.h"
#include "utils/string_utils.h"
#include "core/macros/macros.h"

namespace sore
{
	ProjectData getProjectData(const std::string& filepath)
	{
        using json = nlohmann::json;

        std::ifstream file(filepath);
        if (file.bad())
        {
            // TODO: logging.
            return {};
        }

        if (!endsWith(filepath, Macros::ProjectExtension))
        {
            // TODO: logging
            return {};
        }

        // TODO: json error logging.
        json data = json::parse(file);
        json header = data["header"];
        json source = data["source"];

        // Episodes:
        std::vector<EpisodeMetadata> episodeMetadata;
        for (const auto& episodeData : source["episodes"])
        {        
            episodeMetadata.push_back ({
                episodeData["id"],
                episodeData["filename"],
            });
        }

        // Source:
        ProjectSourceMetadata sourceMetadata;
        sourceMetadata.id = source["id"];
        sourceMetadata.episodes = episodeMetadata;

        // Project:
        ProjectData projectData;
        projectData.version = header["version"];
        projectData.projectName = header["project_name"];
        projectData.rootFolder = header["project_root_folder"];
        projectData.episodeFolderName = header["project_episode_folder"];
        projectData.sourceMetadata = sourceMetadata;

		return projectData;
	}

    void createProjectFile(ProjectData& data)
    {
        data.version = Macros::version;

        namespace fs = std::filesystem;
        using namespace nlohmann;

        json projectFile;

        projectFile["header"] =  {
            { "version", data.version },
            { "project_name", data.projectName },
            { "project_root_folder", data.rootFolder },
            { "project_episode_folder", data.episodeFolderName },
        };
        projectFile["source"] = {};
        projectFile["source"]["id"] = generateUUID();

        auto episodes = nlohmann::json::array();
        for (const auto& episode : data.sourceMetadata.episodes)
        {
            episodes.push_back({
                { "id", episode.id },
                { "filename", episode.filename },
            });
        }
        projectFile["source"]["episodes"] = episodes;

        std::string projectFilename = data.projectName + "." + Macros::ProjectExtension;
        fs::path projectFilePath = fs::path(data.rootFolder) / fs::path(projectFilename);

        std::ofstream outputFile(projectFilePath.string());

        outputFile << projectFile.dump(4, 32, false, json::error_handler_t::ignore);
    }
}