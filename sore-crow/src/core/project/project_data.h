#pragma once
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

#include "components.h"

namespace sore
{
	struct ProjectData
	{
	public:
		ProjectData(const nlohmann::json& jsonObject, const std::string& filepath)
			: projectFilepath(filepath)
		{
			header = ProjectHeader(jsonObject["header"]);
			mediaData = ProjectMediaData(jsonObject["media"]);
		}

		ProjectData() = default;

	public:
		nlohmann::json toJSON() const
		{
			nlohmann::json jsonObject;
			jsonObject["header"] = header.toJSON();
			jsonObject["media"] = mediaData.toJSON(); 

			return jsonObject;
		}

	public:
		ProjectHeader header;
		ProjectMediaData mediaData;
		std::string projectFilepath;
	};

	std::optional<ProjectData> getProjectData(const std::string& filepath);

	void createProjectFile(const ProjectData& data, const std::string& projectDirectory);
}