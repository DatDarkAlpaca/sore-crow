#pragma once
#include <QObject>
#include "core/subtitle/subtitle_factory.h"

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
		void load(const std::string& filepath)
		{
			std::ifstream file(filepath);
			std::stringstream ss;
			ss << file.rdbuf();

			m_LoadedSubtitles = m_SubtitleFactory.getParser(filepath)->parse();
		}

		std::optional<SubtitleData> getClosestSubtitle(uint64_t currentPosition) const
		{
			for (const auto& subtitle : m_LoadedSubtitles)
			{
				if (currentPosition >= subtitle.startTimeMilliseconds && currentPosition <= subtitle.endTimeMilliseconds)
					return subtitle;
			}

			return std::nullopt;
		}

		std::vector<SubtitleData> subtitles() const
		{
			return m_LoadedSubtitles;
		}

	private:
		SubtitleFactory m_SubtitleFactory;
		std::vector<SubtitleData> m_LoadedSubtitles;
	};
}