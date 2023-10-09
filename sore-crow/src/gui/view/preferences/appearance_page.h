#pragma once
#include "core/handlers.h"
#include "utils/string_utils.h"
#include "utils/dialog_utils.h"
#include "ui_appearance_page.h"
#include <QColorDialog>

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
		
			// General Styles:
			populateStyles();
			populateThemes();

			connect(ui.styleComboBox, &QComboBox::currentIndexChanged, this, &AppearancePage::onStyleSelected);
			connect(ui.themeComboBox, &QComboBox::currentIndexChanged, this, &AppearancePage::onThemeSelected);

			// Font Styles:
			populateOverrideSubtitleStyles();
			populateSubtitleOptions();
			connectSubtitleOptions();
		}

	// General Styles:
	private:
		void populateStyles()
		{
			ui.themeComboBox->clear();

			auto& stylesheet = StylesheetHandler::stylesheet;
			auto currentStyle = stylesheet->currentStyle();

			int index = 0;
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

			int index = 0;
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

	// Font Styles:
	private:
		void setFrameColor(QFrame* frame, const QColor& color)
		{
			if (!frame)
				return;

			frame->setStyleSheet(QString("background-color: %1").arg(color.name(QColor::HexArgb)));
		}

		void populateOverrideSubtitleStyles()
		{
			auto* stylesheet = StylesheetHandler::instance();
			ui.overrideStyleButton->setChecked(stylesheet->getSubtitleOverride());

			connect(ui.overrideStyleButton, &QRadioButton::released, [&, stylesheet]() {
				stylesheet->setSubtitleOverride(ui.overrideStyleButton->isChecked());
			});
		}

		void populateSubtitleOptions()
		{
			auto* stylesheet = StylesheetHandler::instance();
			auto fontStyle = stylesheet->getSubtitleFontStyles();
			
			ui.fontFamilyComboBox->setCurrentFont(QFont(fontStyle.family));
			ui.fontSizeSpinBox->setValue(fontStyle.size);
			setFrameColor(ui.foregroundFrame, fontStyle.color);
			setFrameColor(ui.backgroundFrame, fontStyle.backgroundColor);
		}

		void connectSubtitleOptions()
		{
			connect(ui.fontFamilyComboBox, &QFontComboBox::currentFontChanged, [](const QFont& font) {
				auto* stylesheet = StylesheetHandler::instance();
				SubtitleFontStyles currentStyles = stylesheet->getSubtitleFontStyles();
				
				currentStyles.family = font.family();
				stylesheet->setSubtitleFontStyles(currentStyles);
			});

			connect(ui.fontSizeSpinBox, &QSpinBox::valueChanged, [](int size) {
				auto* stylesheet = StylesheetHandler::instance();
				SubtitleFontStyles currentStyles = stylesheet->getSubtitleFontStyles();

				currentStyles.size = size;
				stylesheet->setSubtitleFontStyles(currentStyles);
			});

			connect(ui.foregroundColorBtn, &QPushButton::released, [=]() {
				auto* stylesheet = StylesheetHandler::instance();
				SubtitleFontStyles currentStyles = stylesheet->getSubtitleFontStyles();
				
				auto dialog = QColorDialog(currentStyles.color);
				dialog.setOption(QColorDialog::ShowAlphaChannel, true);
				if (dialog.exec() != QDialog::Accepted)
					return;

				QColor color = dialog.selectedColor();
				if (!color.isValid())
					return;

				setFrameColor(ui.foregroundFrame, color);
				currentStyles.color = color.name(QColor::HexArgb);
				stylesheet->setSubtitleFontStyles(currentStyles);
			});

			connect(ui.backgroundColorBtn, &QPushButton::released, [=]() {
				auto* stylesheet = StylesheetHandler::instance();
				SubtitleFontStyles currentStyles = stylesheet->getSubtitleFontStyles();

				auto dialog = QColorDialog(currentStyles.color);
				dialog.setOption(QColorDialog::ShowAlphaChannel, true);
				if (dialog.exec() != QDialog::Accepted)
					return;

				QColor color = dialog.selectedColor();
				if (!color.isValid())
					return;

				setFrameColor(ui.backgroundFrame, color);
				currentStyles.backgroundColor = color.name(QColor::HexArgb);
				stylesheet->setSubtitleFontStyles(currentStyles);
			});
		}

	private:
		Ui::AppearancePage ui;
	};
}