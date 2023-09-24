#pragma once
#include <memory>
#include <optional>
#include <QStandardItemModel>
#include "project_data.h"
#include "core/subtitle/subtitle_factory.h"

namespace sore
{
	class SubtitleModel : public QStandardItemModel
	{
		Q_OBJECT

	public:
		SubtitleModel(QObject* object = nullptr)
			: QStandardItemModel(object)
		{

		}

	public:
		void populateData(const QString& filepath)
		{
			clear();
			m_Subtitles.reset();

			SubtitleFactory factory(filepath);
			m_Subtitles = factory.parse();

			if (!m_Subtitles)
			{
				// TODO: error.
				return;
			}

			switch (m_Subtitles->type)
			{
				case SubtitleType::SRT:
					populateSRTData();
					break;
					
				case SubtitleType::ASS:
					populateASSData();
					break;
			}
		}

	public:
		std::optional<QModelIndex> getClosestSubtitle(double position)
		{
			for (size_t i = 0; i < rowCount(); ++i)
			{
				auto currentIndex = index(i, 0);
				double startSubtitle = currentIndex.data(Roles::StartRole).toDouble();
				double endSubtitle = currentIndex.data(Roles::EndRole).toDouble();

				if(position >= startSubtitle && endSubtitle >= position)
					return currentIndex;
			}

			return std::nullopt;
		}

	private:
		void populateSRTData()
		{
			srt::Subtitles* subtitles = srt();

			for (const auto& subtitle : subtitles->subtitles)
			{
				QStandardItem* item = new QStandardItem;
				item->setData(subtitle.text, Roles::TextRole);
				item->setData(subtitle.startTimeMs, Roles::StartRole);
				item->setData(subtitle.endTimeMs, Roles::EndRole);
				item->setData((int)SubtitleType::SRT, Roles::TypeRole);

				appendRow(item);
			}
		}

		void populateASSData()
		{
			ass::Subtitles* subtitles = static_cast<ass::Subtitles*>(m_Subtitles.get());

			for (const auto& dialogue : subtitles->dialogues)
			{
				QStandardItem* item = new QStandardItem;
				item->setData(dialogue.text, Roles::TextRole);
				item->setData(dialogue.startMs, Roles::StartRole);
				item->setData(dialogue.endMs, Roles::EndRole);
				item->setData((int)SubtitleType::ASS, Roles::TypeRole);

				appendRow(item);
			}
		}

	public:
		enum Roles
		{
			TextRole = Qt::DisplayRole,
			StartRole = 10,
			EndRole = 11,
			TypeRole = 12
		};

	public:
		ISubtitles* subtitles() const { return m_Subtitles.get(); }

		srt::Subtitles* srt() const { return static_cast<srt::Subtitles*>(m_Subtitles.get()); }

		ass::Subtitles* ass() const { return static_cast<ass::Subtitles*>(m_Subtitles.get()); }

	private:
		std::unique_ptr<ISubtitles> m_Subtitles;
	};
}