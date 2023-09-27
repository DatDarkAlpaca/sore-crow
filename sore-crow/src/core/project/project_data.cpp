#include "pch.h"
#include "project_data.h"
#include "core/handlers.h"

namespace sore 
{
	ProjectData::ProjectData(const QString& filepath, const ProjectHeader& header, const ProjectMediaData& mediaData)
		: filepath(filepath)
		, header(header)
		, mediaData(mediaData)
	{
	}

	ProjectData::ProjectData(const QString& filepath)
	{
		this->filepath = filepath;
		open(filepath);
	}

	void ProjectData::open(const QString& filepath)
	{
		auto object = FilesystemHandler::loadFromJsonFile(filepath);

		if (const QJsonValue value = object["header"]; value.isObject())
			header = parseHeader(value.toObject());

		if (const QJsonValue value = object["media"]; value.isObject())
			mediaData = parseMediaData(value.toObject());
	}

	void ProjectData::save(const QString& filepath) const
	{
		QFile file(filepath);
		file.open(QFile::WriteOnly);

		QJsonDocument document(toJSON());
		file.write(document.toJson());
	}

	void ProjectData::addMedia(const QString& filepath)
	{
		mediaData.episodeData.push_back(EpisodeData(filepath));
	}

	void ProjectData::removeMedia(const QUuid& id)
	{
		auto& episodeData = mediaData.episodeData;

		episodeData.erase(std::remove_if(episodeData.begin(), episodeData.end(), [&](const EpisodeData& episode) {
			return episode.id == id;
		}));
	}

	QJsonObject ProjectData::toJSON() const
	{
		QJsonObject root;

		QJsonObject headerObject;
		headerObject["project_name"] = header.projectName;
		headerObject["version"] = header.projectVersion;

		QJsonArray episodesObject;
		for (const auto& media : mediaData.episodeData)
		{
			QJsonObject mediaObject;
			mediaObject["id"] = media.id.toString();
			mediaObject["filepath"] = media.filepath;

			episodesObject.push_back(mediaObject);
		}

		QJsonObject mediaDataObject;
		mediaDataObject["episodes"] = episodesObject;

		root["header"] = headerObject;
		root["media"] = mediaDataObject;

		return root;
	}

	ProjectHeader ProjectData::parseHeader(const QJsonObject& object)
	{
		ProjectHeader header;

		if (const QJsonValue value = object["project_name"]; value.isString())
			header.projectName = value.toString();

		if (const QJsonValue value = object["version"]; value.isString())
			header.projectVersion = value.toString();

		return header;
	}

	ProjectMediaData ProjectData::parseMediaData(const QJsonObject& object)
	{
		ProjectMediaData mediaData;

		const QJsonValue value = object["episodes"];
		if (!value.isArray())
			return {};

		mediaData.episodeData.reserve(value.toArray().size());
		for (const QJsonValue& media : value.toArray())
		{
			EpisodeData data;
			if (!media.isObject())
				continue;

			data = parseEpisodeData(media.toObject());
			mediaData.episodeData.push_back(data);
		}

		return mediaData;
	}

	EpisodeData ProjectData::parseEpisodeData(const QJsonObject& object)
	{
		EpisodeData episodeData;

		if (const QJsonValue value = object["id"]; value.isString())
			episodeData.id = QUuid::fromString(value.toString());

		if (const QJsonValue value = object["filepath"]; value.isString())
			episodeData.filepath = value.toString();

		return episodeData;
	}
}