#pragma once
#include <QGraphicsView>
#include "gui/components/subtitle_item/subtitle_item.h"

namespace sore
{
	class SelectableSubtitles : public QGraphicsView
	{
	public:
		SelectableSubtitles(QWidget* parent = nullptr)
			: QGraphicsView(parent)
		{
			setAttribute(Qt::WA_NoSystemBackground);
			setScene(m_Scene);

			m_Scene = new QGraphicsScene(this);
			m_PrimarySubtitle = new SubtitleItem("Hello");
			m_SecondarySubtitle = new SubtitleItem("World");
			
			m_Scene->addItem(m_PrimarySubtitle);
			m_Scene->addItem(m_SecondarySubtitle);

			setScene(m_Scene);
		}

	private:
		QGraphicsScene* m_Scene = nullptr;
		SubtitleItem* m_PrimarySubtitle = nullptr;
		SubtitleItem* m_SecondarySubtitle = nullptr;
	};
}