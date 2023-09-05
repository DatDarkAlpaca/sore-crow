#pragma once
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

	private:
		CrowVideo& m_CrowVideo;

	};
}