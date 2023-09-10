#pragma once
#include <QApplication>
#include <string>

namespace sore
{
	struct Macros
	{
		constexpr static inline const char* version = "0.1";

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

	static inline std::string ResourcesPath;
	static inline std::string StylePath;
	static inline std::string StyleOutputPath;
	static inline std::string PluginPath;
	static inline std::string WorkingDirectory;
	static inline int DefaultSubtitlePointSize = 20;

	struct Data
	{
	public:
		static void initialize()
		{
			namespace fs = std::filesystem;
			auto appPath = qApp->applicationDirPath().toStdString();

			ResourcesPath = appPath + "/res";
			StylePath = appPath + "/res/styles";
			StyleOutputPath = appPath + "/res/output";
			PluginPath = appPath + "/plugins";
			WorkingDirectory = std::filesystem::current_path().string();
		}

	public:
		bool isStylesheetDark = true;
	} inline globalData;
}