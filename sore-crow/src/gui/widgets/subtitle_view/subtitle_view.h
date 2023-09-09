#pragma once
#include <QMenu>
#include <QListView>
#include <QClipboard>
#include <QAbstractListModel>
#include "gui/model/subtitle_model.h"

namespace sore
{
	class SubtitleListView : public QListView
	{
		Q_OBJECT

	public:
		SubtitleListView(QWidget* parent = nullptr)
		{
			setUniformItemSizes(true);
		}

	public:
		void showContextMenu(const QPoint& pos, const SubtitleModel& model)
		{
			QPoint item = mapToGlobal(pos);
			
			QMenu contextMenu;
			contextMenu.addAction("Copy");

			QAction* rightClickItem = contextMenu.exec(item);
			if (rightClickItem && rightClickItem->text().contains("Copy"))
			{
				auto modelRow = indexAt(pos).row();
				auto modelData = model.index(modelRow, 0).data().toString();

				QClipboard* clipboard = QGuiApplication::clipboard();
				clipboard->setText(modelData);
			}
		}
	};
}