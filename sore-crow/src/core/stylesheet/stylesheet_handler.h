#pragma once
#include <memory>
#include <QtAdvancedStylesheet.h>

namespace sore
{
	class StylesheetHandler
	{
	public:
		static void initialize();

	public:
		static bool styleExists(const QString& styleName);

		static bool themeExists(const QString& themeName);

		static void selectStyle(const QString& styleName);

		static void selectTheme(const QString& themeName);

		static void applyStylesheet();

	private:
		static void initializeDefaultStylesheet();

		static void updateSettings();

	public:
		static inline std::unique_ptr<acss::QtAdvancedStylesheet> stylesheet;
	};
}