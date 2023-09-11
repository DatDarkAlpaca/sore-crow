#pragma once
#include <string>
#include "metadata.h"

namespace sore
{
	struct ProjectData
	{
	public:
		EpisodeMetadata getEpisodeFromID(const std::string& episodeID)
		{
			for (const auto& episode : sourceMetadata.episodes)
			{
				if (episode.id == episodeID)
					return episode;
			}

			return { };
		}

	public:
		bool valid() const
		{
			return !version.empty() && !projectName.empty() && !rootFolder.empty() && 
				!episodeFolderName.empty() && !sourceMetadata.id.empty();
		}

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