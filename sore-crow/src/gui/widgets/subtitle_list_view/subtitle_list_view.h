#pragma once
#include <QListView>
#include "gui/model/subtitle_model.h"

namespace sore
{
	class SubtitleListView : public QListView
	{
		Q_OBJECT

	public:
		SubtitleListView(QWidget* parent = nullptr)
			: QListView(parent)
		{
			setContextMenuPolicy(Qt::CustomContextMenu);
			connect(this, &SubtitleListView::customContextMenuRequested, this, &SubtitleListView::showContextMenu);
		}

	private:
		void showContextMenu(const QPoint& position)
		{
			QPoint item = mapToGlobal(position);
			m_IsContextOpen = true;

			// Sets the current index to the user's index, in case the position was changed abruptly:
			auto index = indexAt(position);
			setCurrentIndex(index);

			// Creates the menu:
			QMenu contextMenu(tr("Subtitle Tools"), this);
			contextMenu.addAction("Jump to timestamp");
			contextMenu.addAction("Copy Subtitle");

			QAction* rightClickItem = contextMenu.exec(item);
			if (rightClickItem && rightClickItem->text().contains("Jump to timestamp"))
			{
				auto index = indexAt(position);
				double start = index.data(SubtitleModel::Roles::StartRole).toDouble();
				emit jumpedToTimestamp(start);
			}

			else if (rightClickItem && rightClickItem->text().contains("Copy Subtitle"))
			{
				QClipboard* clipboard = qApp->clipboard();

				auto index = indexAt(position);
				QString text = index.data(SubtitleModel::Roles::TextRole).toString();

				clipboard->setText(text);
			}

			connect(&contextMenu, &QMenu::destroyed, [&]() {
				m_IsContextOpen = false;
			});
		}

	signals:
		void jumpedToTimestamp(double startPosition);

	public:
		bool contextMenuOpen() const { return m_IsContextOpen; }

	private:
		bool m_IsContextOpen = false;
	};
}