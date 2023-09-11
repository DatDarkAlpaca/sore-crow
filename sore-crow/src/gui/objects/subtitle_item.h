#pragma once
#include <Qt>
#include <QPainter>
#include <QTextCursor>
#include <QGraphicsTextItem>

namespace sore
{
	class SubtitleItem : public QGraphicsTextItem
	{
		Q_OBJECT

	public:
		SubtitleItem(QGraphicsItem* parent) 
			: QGraphicsTextItem(parent)
		{
			setTextInteractionFlags(Qt::TextSelectableByMouse);

			onSelectedTextChanges();
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

	public:
		void onSelectedTextChanges()
		{
			auto selectedText = this->textCursor().selectedText();
			emit textSelected(selectedText);
		}

		void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override
		{
			onSelectedTextChanges();
			QGraphicsTextItem::mouseDoubleClickEvent(event);
		}

		void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override
		{
			onSelectedTextChanges();
			QGraphicsTextItem::mouseMoveEvent(event);
		}

		void mousePressEvent(QGraphicsSceneMouseEvent* event) override
		{
			onSelectedTextChanges();
			QGraphicsTextItem::mousePressEvent(event);
		}

		void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override
		{
			onSelectedTextChanges();
			QGraphicsTextItem::mouseReleaseEvent(event);
		}

	signals:
		void textSelected(const QString&);

	private:
		QColor m_BackgroundColor = QColor(0, 0, 0, 0);
	};
}