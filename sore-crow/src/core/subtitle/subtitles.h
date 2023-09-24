#pragma once

namespace sore
{
	enum class SubtitleType { NONE = 0, SRT, ASS };

	class ISubtitles
	{
	public:
		ISubtitles(SubtitleType type)
			: type(type)
		{

		}

	public:
		virtual ~ISubtitles() = default;

	public:
		SubtitleType type = SubtitleType::NONE;
	};
}