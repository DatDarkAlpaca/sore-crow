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
		}

	public:
		void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) 
		{
			painter->setBrush(m_BackgroundColor);
			painter->drawRect(boundingRect());
			QGraphicsTextItem::paint(painter, option, widget);
		}

	public:
		void setBackgroundColor(const QColor& color)
		{
			m_BackgroundColor = color;
		}

	private:
		QColor m_BackgroundColor = QColor(0, 0, 0, 0);
	};
}