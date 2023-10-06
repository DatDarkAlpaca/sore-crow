#pragma once
#include <QStandardItemModel>
#include "core/project/project_data.h"
#include "core/handlers.h"

namespace sore
{
	class EpisodeModel : public QStandardItemModel
	{
		Q_OBJECT

	public:
		EpisodeModel(QObject* object = nullptr)
			: QStandardItemModel(object)
		{

		}

	public:
		void populateData(const ProjectMediaData& mediaData)
		{
			QString projectDirectory = SettingsHandler::settings->getString("project/directory");

			clear();
			
			size_t index = 0;
			for (const auto& episode : mediaData.episodeData)
			{
				auto episodeName = FilesystemHandler::getFilename(episode.filepath);

				QStandardItem* item = new QStandardItem;
				item->setData(episodeName, Roles::NameRole);
				item->setData(FilesystemHandler::getMediaFilepath(episode.filepath), Roles::FilepathRole);
				item->setData(index, Roles::IndexRole);
				item->setData(episode.id, Roles::IDRole);

				appendRow(item);
				++index;
			}
		}

	public:
		enum Roles
		{
			NameRole = Qt::DisplayRole,
			FilepathRole = 10,
			IndexRole = 20,
			IDRole = 30,
		};
	};
}