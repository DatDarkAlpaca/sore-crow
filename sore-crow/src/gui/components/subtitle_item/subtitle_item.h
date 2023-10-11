#pragma once
#include <QGraphicsTextItem>

namespace sore
{
	class SubtitleItem : public QGraphicsTextItem
	{
	public:
		SubtitleItem(const QString& text, QGraphicsTextItem* parent = nullptr)
			: QGraphicsTextItem(text, parent)
		{
			setFlag(QGraphicsItem::ItemIsMovable, true);
			setFlag(QGraphicsItem::ItemIsSelectable, true);
			setFlag(QGraphicsItem::ItemClipsToShape, true);
		}

	public:

	};
}