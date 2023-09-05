#pragma once
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QGraphicsVideoItem>

namespace sore
{
	class CrowVideo : public QGraphicsView
	{
		Q_OBJECT

	public:
		CrowVideo(QWidget* parent = nullptr)
			: QGraphicsView(parent)
		{
			m_Scene = new QGraphicsScene(parent);
			setScene(m_Scene);

			m_VideoItem = new QGraphicsVideoItem;
			m_SubtitleItem = new QGraphicsTextItem;

			m_VideoItem->setZValue(0);
			m_SubtitleItem->setZValue(20);

			m_VideoItem->setPos(0, 0);
			m_SubtitleItem->setPos(0, 0);

			m_Scene->addItem(m_SubtitleItem);
			m_Scene->addItem(m_VideoItem);

			m_SubtitleItem->setPlainText("hello");
			setStyleSheet("border: 0;");
		}

	public:
		void setFont(const QFont& font)
		{
			m_SubtitleItem->setFont(font);
		}

		void setText(const std::string& text)
		{
			m_SubtitleItem->setPlainText(text.c_str());
		}

	protected:
		void resizeEvent(QResizeEvent* event) override
		{
			fitInView(m_Scene->sceneRect(), Qt::KeepAspectRatio);
			m_VideoItem->setSize({ m_Scene->width(), m_Scene->height() });
		}

	public:
		QGraphicsVideoItem* videoItem() const { return m_VideoItem; }

	private:
		QGraphicsTextItem* m_SubtitleItem = nullptr;
		QGraphicsVideoItem* m_VideoItem = nullptr;
		QGraphicsScene* m_Scene = nullptr;
	};
}