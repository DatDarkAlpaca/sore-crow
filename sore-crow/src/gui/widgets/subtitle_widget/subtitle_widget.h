#pragma once
#include <QFileInfo>
#include <QMouseEvent>
#include <QClipboard>
#include <QtWidgets/QWidget>

#include "ui_subtitle.h"
#include "core/project/metadata.h"
#include "utils/video_utils.h"

namespace sore
{
	class SubtitleWidget : public QWidget
	{
		Q_OBJECT

	public:
		SubtitleWidget(QWidget* parent = nullptr)
		{
			ui.setupUi(this);
		}

	public:
		void updateData(const std::string& subtitleText)
		{
			ui.subtitle->setText(subtitleText.c_str());
			
			QObject::connect(ui.copyBtn, QPushButton::released, [&]() {
				QClipboard* clipboard = QApplication::clipboard();
				clipboard->setText(ui.subtitle->text(), QClipboard::Clipboard);
			});
		}

	private:
		void mousePressEvent(QMouseEvent* event) override
		{
			if (event->button() == Qt::LeftButton)
				emit subtitleClicked();

			QWidget::mousePressEvent(event);
		}

	signals:
		void subtitleClicked();

	private:
		Ui::SubtitleWidget ui;
	};
}