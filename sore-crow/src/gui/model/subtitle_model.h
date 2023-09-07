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
		void populateData(const std::vector<SubtitleData>& data)
		{
			clear();

			for (const auto& subtitle : data)
			{
				QStandardItem* item = new QStandardItem;
				item->setData(subtitle.text.c_str(), Qt::DisplayRole);

				appendRow(item);
				auto row = item->index().row();
			}

			m_Data = data;
		}

	public:
		std::optional<std::pair<size_t, SubtitleData>> getDataAndRowAtPosition(uint64_t position)
		{
			for(size_t i = 0; i < m_Data.size(); ++i)
			{
				if (m_Data[i].startTimeMilliseconds == position)
					return std::make_pair(i, m_Data[i]);
			}

			return std::nullopt;
		}

		SubtitleData getDataAtModelIndex(int row) { return m_Data[row]; }

		std::vector<SubtitleData> getSubtitleData() const { return m_Data; }

	private:
		std::vector<SubtitleData> m_Data;
	};
}