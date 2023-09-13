#pragma once
#include <QApplication>
#include <string>

namespace sore
{
	struct Macros
	{
		constexpr static inline const char* version = "0.1";

		constexpr static inline const char* DefaultEpisodesFolderName = "episodes";

		constexpr static inline std::array<const char*, 2> supportedVideoFormats = {
			"mkv", "mp4"
		};

		constexpr static inline const char* ProjectExtension = "prj";
	};

	inline std::string ResourcesPath;
	inline std::string StylePath;
	inline std::string StyleOutputPath;
	inline std::string PluginPath;
	inline std::string WorkingDirectory;
	inline std::string ProjectDirectory;
	inline int DefaultSubtitlePointSize = 20;

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