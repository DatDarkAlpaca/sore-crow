#pragma once
#include <QUuid>
#include <QJsonObject>
#include "constants.h"

namespace sore
{
	struct EpisodeData
	{
	public:
		EpisodeData() = default;

		EpisodeData(const QString& filepath)
			: filepath(filepath) 
		{
			id = QUuid::createUuid();
		}

	public:
		QUuid id;
		QString filepath;
	};

	struct ProjectHeader
	{
	public:
		ProjectHeader(const QString& name)
			: projectName(name)
		{
			projectVersion = ProjectVersion;
		}

		ProjectHeader() = default;

	public:
		QString projectName;
		QString projectVersion;
	};

	struct ProjectMediaData
	{
		std::vector<EpisodeData> episodeData;
	};

	class ProjectData
	{
		Q_GADGET

	public:
		ProjectData(const ProjectHeader& header, const ProjectMediaData& mediaData);

		ProjectData(const QString& filepath);

		ProjectData() = default;

		~ProjectData() = default;

	public:
		void open(const QString& filepath);

		void save(const QString& filepath) const;

	public:
		QJsonObject toJSON() const;

	private:
		static ProjectHeader parseHeader(const QJsonObject& object);

		static ProjectMediaData parseMediaData(const QJsonObject& mediaArray);

		static EpisodeData parseEpisodeData(const QJsonObject& object);

	public:
		ProjectHeader header = {};
		ProjectMediaData mediaData = {};
	};
}