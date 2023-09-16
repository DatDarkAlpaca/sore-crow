#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "crow_settings.h"
#include "utils/uuid_utils.h"

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
			jsonObject["last_episode_selected"].get_to(lastEpisodeSelected);
			jsonObject["last_episode_position"].get_to(lastEpisodePosition);
		}

		ProjectHeader()
		{
			// God forgive me for this hack.
			projectVersion = (settings) ? settings->value("version").toString().toStdString() : "u.u";
		}

	public:
		nlohmann::json toJSON() const
		{
			nlohmann::json jsonObject;
			jsonObject["project_name"] = projectName;
			jsonObject["version"] = projectVersion;
			jsonObject["last_episode_selected"] = lastEpisodeSelected;
			jsonObject["last_episode_position"] = lastEpisodePosition;

			return jsonObject;
		}

	public:
		std::string projectName;
		std::string projectVersion;
		std::string lastEpisodeSelected;
		long long lastEpisodePosition;
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