#pragma once
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

#include "components.h"

namespace sore
{
	class ProjectData
	{
	public:
		ProjectData(const nlohmann::json& jsonObject, const std::string& filepath);

		ProjectData() = default;

	public:
		bool open(const std::string& filepath);

		void update(const std::string& section, const std::string& field, const nlohmann::json& object);

		void save();

	private:
		nlohmann::json toJSON() const;

		void setData(const nlohmann::json& object);

	public:
		ProjectHeader header;
		ProjectMediaData mediaData;
		std::string projectFilepath;
	};
}