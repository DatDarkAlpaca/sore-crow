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

			auto subtitleData = m_SubtitleFactory.getParser(filepath)->parse();
			for (const auto& subtitle : subtitleData)
				m_LoadedSubtitles[subtitle.startTimeMilliseconds] = subtitle;
		}

		std::optional<SubtitleData> getClosestSubtitle(uint64_t currentPosition) const
		{
			for (const auto& [position, data] : m_LoadedSubtitles)
			{
				if (currentPosition >= position && currentPosition <= data.endTimeMilliseconds)
					return data;
			}

			return std::nullopt;
		}

		std::unordered_map<uint64_t, SubtitleData> subtitles() const
		{
			return m_LoadedSubtitles;
		}

	private:
		SubtitleFactory m_SubtitleFactory;
		std::unordered_map<uint64_t, SubtitleData> m_LoadedSubtitles;
	};
}