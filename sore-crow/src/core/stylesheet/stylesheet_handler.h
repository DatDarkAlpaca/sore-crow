#pragma once
#include <memory>
#include <QtAdvancedStylesheet.h>
#include "subtitle_font_styles.h"

namespace sore
{
	class StylesheetHandler : public QObject
	{
		Q_OBJECT

	private:
		StylesheetHandler(QObject* parent = nullptr);

	public:
		static void initialize();

		static void initializePlayerStyles();

	public:
		static bool styleExists(const QString& styleName);

		static bool themeExists(const QString& themeName);

		static void selectStyle(const QString& styleName);

		static void selectTheme(const QString& themeName);

		static void applyStylesheet();

	public:
		void setSubtitleOverride(bool enabled);

		bool getSubtitleOverride();

		SubtitleFontStyles getSubtitleFontStyles();

		void setSubtitleFontStyles(const SubtitleFontStyles& styles);

	private:
		static void initializeDefaultStylesheet();

		static void updateSettings();

	signals:
		void overrideStylesChanged(bool overrideEnabled);

		void subtitleStyleChanged(const SubtitleFontStyles&);

	private:
		StylesheetHandler(StylesheetHandler& other) = delete;

		void operator=(const StylesheetHandler&) = delete;

	public:
		static inline StylesheetHandler* instance() { return s_Instance; }

	public:
		static inline std::unique_ptr<acss::QtAdvancedStylesheet> stylesheet;
		static inline StylesheetHandler* s_Instance = nullptr;
	};
}