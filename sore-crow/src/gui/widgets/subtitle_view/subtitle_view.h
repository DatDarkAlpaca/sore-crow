#pragma once
#include <QListView>
#include <QAbstractListModel>

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
	};
}