#pragma once
#include <string>
#include <vector>

namespace sore
{
	struct EpisodeMetadata
	{
		std::string id;
		std::string subtitleID;
		std::string filename;
	};

	struct SubtitleMetadata
	{
		std::string id;
		std::string episodeID;
		std::string filename;
	};

	struct ProjectSourceMetadata
	{
		std::string id;
		std::vector<EpisodeMetadata> episodes;
		std::vector<SubtitleMetadata> subtitles;
	};
}