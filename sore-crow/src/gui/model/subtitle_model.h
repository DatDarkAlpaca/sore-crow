#pragma once
#include <unordered_map>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include "core/subtitle/subtitle_data.h"

namespace sore
{
	class SubtitleModel : public QStandardItemModel
	{
		Q_OBJECT

	public:
		SubtitleModel(QObject* parent = nullptr)
			: QStandardItemModel(parent)
		{

		}

	public:
		void populateData(const std::unordered_map<uint64_t, SubtitleData>& data)
		{
			clear();

			for (const auto& [_, subtitle] : data)
			{
				QStandardItem* item = new QStandardItem;
				item->setData(subtitle.text.c_str(), Qt::DisplayRole);
				appendRow(item);
			}
		}
	};
}