#pragma once
#include <array>
#include <string>

namespace sore
{
	struct Macros
	{
		constexpr static inline const char* version = "0.1";

		constexpr static inline std::array<const char*, 2> AcceptableSubtitleFolderNames = {
			"subtitles", "subtitle"
		};

		constexpr static inline std::array<const char*, 2> AcceptableEpisodeFolderNames = {
			"episodes", "episode"
		};

		constexpr static inline std::array<const char*, 2> supportedSubtitleFormats = {
			"srt", "ass"
		};

		constexpr static inline std::array<const char*, 2> supportedVideoFormats = {
			"mkv", "mp4"
		};

		constexpr static inline const char* ProjectExtension = "prj";
	};
}