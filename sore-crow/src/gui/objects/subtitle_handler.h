#pragma once
#include <unordered_map>
#include "core/parser/objects.h"
#include "gui/widgets/crow_video/crow_video.h"

namespace sore
{
	class SubtitleHandler
	{
	public:
		SubtitleHandler(CrowVideo& crowVideo)
			: m_CrowVideo(crowVideo)
		{
			
		}

	public:
		void setSubtitleFiles(const std::vector<SubtitleEntry>& subtitles) 
		{
			for (const auto& subtitle : subtitles)
				qDebug() << subtitle.counter;
			m_SubtitleFiles = subtitles;
		}
		
	private:
		std::vector<SubtitleEntry> m_SubtitleFiles;
		CrowVideo& m_CrowVideo;
	};
}