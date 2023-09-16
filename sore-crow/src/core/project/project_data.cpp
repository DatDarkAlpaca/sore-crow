#include "pch.h"
#include "project_data.h"
#include "crow_settings.h"

#include "utils/string_utils.h"
#include "utils/message_box.h"
#include "core/logger/logger.h"

namespace sore
{
	ProjectData::ProjectData(const nlohmann::json& jsonObject, const std::string& filepath)
		: projectFilepath(filepath)
	{
		header = ProjectHeader(jsonObject["header"]);
		mediaData = ProjectMediaData(jsonObject["media"]);
	}

	bool ProjectData::open(const std::string& filepath)
	{
		using namespace nlohmann;

		projectFilepath = filepath;
		auto projectExtension = settings->value("project/extension", "prj").toString().toStdString();

		std::ifstream file(filepath);
		if (file.bad())
		{
			CrownsoleLogger::log("Failed to retrieve project data from " + filepath + ".", Severity::ERROR);
			return false;
		}

		if (!endsWith(filepath, projectExtension))
		{
			CrownsoleLogger::log("This project file ends in the wrong extension.", Severity::ERROR);
			return false;
		}

		try
		{
			auto jsonObject = json::parse(file);
			setData(jsonObject);
			return true;
		}
		catch (nlohmann::json::exception e) {
			CrownsoleLogger::log(e.what(), Severity::ERROR);
			errorBox(e.what());
			return false;
		}
	}

	void ProjectData::update(const std::string& section, const std::string& field, const nlohmann::json& object)
	{
		using namespace nlohmann;

		auto jsonData = toJSON();
		try
		{
			jsonData.at(section).at(field) = object;
			setData(jsonData);
		}
		catch (const json::exception& e)
		{
			std::cerr << "Failed to update project data: " << e.what() << '\n';
		}
	}

	void ProjectData::save()
	{
		namespace fs = std::filesystem;
		using namespace nlohmann;

		if (projectFilepath.empty())
		{
			errorBox("Project does not have a filepath set. Please contact the developer.");
			return;
		}

		auto jsonData = toJSON();
		std::ofstream outputFile(projectFilepath);
		outputFile << jsonData.dump(4, 32, false, json::error_handler_t::ignore);
	}

	nlohmann::json ProjectData::toJSON() const
	{
		nlohmann::json jsonObject;
		jsonObject["header"] = header.toJSON();
		jsonObject["media"] = mediaData.toJSON();

		return jsonObject;
	}

	void ProjectData::setData(const nlohmann::json& object)
	{
		using namespace nlohmann;

		header = ProjectHeader(object["header"]);
		mediaData = ProjectMediaData(object["media"]);
	}
}