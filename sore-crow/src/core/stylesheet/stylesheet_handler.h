#pragma once
#include <memory>
#include <QtAdvancedStylesheet.h>

namespace sore
{
	class StylesheetHandler
	{
	public:
		static void initialize();

	private:
		static void initializeDefaultStylesheet();

		static void updateSettings();

	public:
		static inline std::unique_ptr<acss::QtAdvancedStylesheet> stylesheet;
	};
}