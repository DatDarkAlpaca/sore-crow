#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "utils/uuid_utils.h"
#include "core/global_data.h"

namespace sore
{
	struct EpisodeData
	{
	public:
		EpisodeData(const nlohmann::json& jsonObject)
		{
			jsonObject.at("id").get_to(id);
			jsonObject.at("filepath").get_to(filepath);
		}

		EpisodeData()
		{
			id = generateUUID();
		}

	public:
		nlohmann::json toJSON() const
		{
			nlohmann::json object;
			object["id"] = id;
			object["filepath"] = filepath;

			return object;
		}

	public:
		std::string id;
		std::string filepath;
	};

	struct ProjectHeader
	{
	public:
		ProjectHeader(const nlohmann::json& jsonObject)
		{
			jsonObject["project_name"].get_to(projectName);
			jsonObject["version"].get_to(projectVersion);
		}

		ProjectHeader()
		{
			projectVersion = Macros::version;
		}

	public:
		nlohmann::json toJSON() const
		{
			nlohmann::json jsonObject;
			jsonObject["project_name"] = projectName;
			jsonObject["version"] = projectVersion;

			return jsonObject;
		}

	public:
		std::string projectName;
		std::string projectVersion;
	};

	struct ProjectMediaData
	{
	public:
		ProjectMediaData(const nlohmann::json& jsonObject)
		{
			for (const auto& episode : jsonObject["episodes"])
			{
				EpisodeData parsedEpisode(episode);
				episodeData.push_back(parsedEpisode);
			}
		}

		ProjectMediaData() = default;

	public:
		nlohmann::json toJSON() const
		{
			nlohmann::json jsonObject;

			auto episodes = nlohmann::json::array();
			for (const EpisodeData& episode : episodeData)
				episodes.push_back(episode.toJSON());

			jsonObject["episodes"] = episodes;
			return jsonObject;
		}

	public:
		std::vector<EpisodeData> episodeData;
	};
}