#pragma once
#include <QGraphicsTextItem>
#include <QPainter>

namespace sore
{
	class SubtitleItem : public QGraphicsTextItem
	{
	public:
		SubtitleItem(QGraphicsItem* parent) 
			: QGraphicsTextItem(parent)
		{

		}

	public:
		void setBackgroundColor(const QColor& color)
		{
			m_BackgroundColor = color;
		}

		void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
		{
			if (!toPlainText().isEmpty())
			{
				QBrush brush;
				brush.setColor(m_BackgroundColor);
				brush.setStyle(Qt::SolidPattern);

				painter->setPen(Qt::PenStyle::NoPen);
				painter->setBrush(brush);
				painter->drawRect(boundingRect());
			}

			QGraphicsTextItem::paint(painter, option, widget);
		}

	private:
		QColor m_BackgroundColor = QColor(0, 0, 0, 0);
	};
}