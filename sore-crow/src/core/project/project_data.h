#pragma once
#include <string>
#include "metadata.h"

namespace sore
{
	struct ProjectData
	{
	public:
		EpisodeMetadata getEpisodeFromID(const std::string& episodeID);

	public:
		bool valid() const;

	public:
		std::string version;
		std::string projectName;

		std::string rootFolder;
		std::string episodeFolderName;

		ProjectSourceMetadata sourceMetadata;
	};

	std::optional<ProjectData> getProjectData(const std::string& filepath);

	void createProjectFile(ProjectData& data);
}