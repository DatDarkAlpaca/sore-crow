#pragma once
#include "core/handlers.h"
#include "utils/string_utils.h"
#include "utils/dialog_utils.h"
#include "ui_appearance_page.h"

namespace sore
{
	class AppearancePage : public QWidget
	{
	private:
		enum ThemeRole
		{
			DisplayRole = Qt::UserRole,
			StyleThemeNameRole = 10
		};

	public:
		AppearancePage(QWidget* parent = nullptr)
			: QWidget(parent)
		{
			ui.setupUi(this);
		
			populateStyles();
			populateThemes();

			connect(ui.styleComboBox, &QComboBox::currentIndexChanged, this, &AppearancePage::onStyleSelected);
			connect(ui.themeComboBox, &QComboBox::currentIndexChanged, this, &AppearancePage::onThemeSelected);
		}

	private:
		void populateStyles()
		{
			ui.themeComboBox->clear();

			auto& stylesheet = StylesheetHandler::stylesheet;
			auto currentStyle = stylesheet->currentStyle();

			size_t index = 0;
			for (const QString& style : stylesheet->styles())
			{
				QString title = style;
				title.replace("_", " ");
				title = toCamelCase(title);

				ui.styleComboBox->addItem(title);
				ui.styleComboBox->setItemData(index, style, ThemeRole::StyleThemeNameRole);

				if (style == currentStyle)
					ui.styleComboBox->setCurrentIndex(index);

				++index;
			}
		}

		void populateThemes()
		{
			ui.themeComboBox->clear();

			auto& stylesheet = StylesheetHandler::stylesheet;
			auto currentTheme = stylesheet->currentTheme();

			size_t index = 0;
			for (const QString& theme : stylesheet->themes())
			{
				QString title = theme;
				title.replace("_", " ");
				title = toCamelCase(title);

				ui.themeComboBox->addItem(title);
				ui.themeComboBox->setItemData(index, theme, ThemeRole::StyleThemeNameRole);

				if (theme == currentTheme)
					ui.themeComboBox->setCurrentIndex(index);

				++index;
			}
		}

		void onStyleSelected(int index)
		{
			QString styleName = ui.styleComboBox->itemData(index, ThemeRole::StyleThemeNameRole).toString();
			StylesheetHandler::selectStyle(styleName);
			populateThemes();
		}

		void onThemeSelected(int index)
		{
			QString themeName = ui.themeComboBox->itemData(index, ThemeRole::StyleThemeNameRole).toString();
			StylesheetHandler::selectTheme(themeName);
			StylesheetHandler::applyStylesheet();
		}

	private:
		Ui::AppearancePage ui;
	};
}