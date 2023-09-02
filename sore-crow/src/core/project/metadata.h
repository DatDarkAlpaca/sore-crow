#pragma once
#include <string>
#include <vector>

namespace sore
{
	struct EpisodeMetadata
	{
		std::string id;
		std::string filename;
	};

	struct ProjectSourceMetadata
	{
		std::string id;
		std::vector<EpisodeMetadata> episodes;
	};
}