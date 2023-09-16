#pragma once
#include <QObject>
#include "core/subtitle/subtitle_factory.h"
#include "utils/message_box.h"

namespace sore
{
	class SubtitleHandler : public QObject
	{
		Q_OBJECT

	public:
		SubtitleHandler(QObject* parent = nullptr)
			: QObject(parent)
		{

		}

	public:
		void setSubtitleIndex(size_t index) { m_CurrentIndex = index; }

		std::optional<size_t> load(const std::string& filepath)
		{
			std::ifstream file(filepath);
			std::stringstream ss;
			ss << file.rdbuf();

			auto parser = m_SubtitleFactory.getParser(filepath);
			if (!parser)
			{
				errorBox("This subtitle file is invalid or not yet supported.");
				return std::nullopt;
			}

			m_LoadedSubtitles.push_back(parser->parse());
			m_CurrentIndex = m_LoadedSubtitles.size() - 1;

			return m_CurrentIndex;
		}

		std::optional<SubtitleData> getClosestSubtitle(uint64_t currentPosition, size_t index) const
		{
			for (const auto& subtitle : m_LoadedSubtitles[index])
			{
				if (currentPosition >= subtitle.startTimeMilliseconds && currentPosition <= subtitle.endTimeMilliseconds)
					return subtitle;
			}

			return std::nullopt;
		}

		std::vector<SubtitleData> subtitles(size_t index) const
		{
			return m_LoadedSubtitles[index];
		}

		size_t currentIndex() const { return m_CurrentIndex; }

	private:
		std::vector<std::vector<SubtitleData>> m_LoadedSubtitles;
		SubtitleFactory m_SubtitleFactory;
		size_t m_CurrentIndex = 0;
	};
}