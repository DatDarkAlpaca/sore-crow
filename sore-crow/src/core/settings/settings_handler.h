#pragma once
#include <memory>
#include "crow_settings.h"

namespace sore
{
	class SettingsHandler
	{
	public:
		static void initialize();

	private:
		static void initializeDefaultSettings();

	public:
		static inline std::unique_ptr<CrowSettings> settings;

		static inline QString settingsPath;
	};
}