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

		SubtitleMetadata getSubtitleFromID(const std::string& subtitleID)
		{
			for (const auto& subtitle : sourceMetadata.subtitles)
			{
				if (subtitle.id == subtitleID)
					return subtitle;
			}

			return { };
		}

	public:
		bool valid() const
		{
			return !version.empty() && !projectName.empty() && !rootFolder.empty() && !episodeFolderName.empty() && !subtitleFolderName.empty() &&
				!sourceMetadata.id.empty();
		}

	public:
		std::string version;
		std::string projectName;

		std::string rootFolder;
		std::string episodeFolderName;
		std::string subtitleFolderName;

		ProjectSourceMetadata sourceMetadata;
	};

	ProjectData getProjectData(const std::string& filepath);

	void createProjectFile(ProjectData& data);
}