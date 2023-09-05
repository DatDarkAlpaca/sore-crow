#include "pch.h"
#include "crow_video.h"

namespace sore
{
	CrowVideo::CrowVideo(QWidget* parent)
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

	void CrowVideo::setFont(const QFont& font)
	{
		m_SubtitleItem->setFont(font);
	}

	void CrowVideo::setText(const std::string& text)
	{
		m_SubtitleItem->setPlainText(text.c_str());
	}

	void CrowVideo::resizeEvent(QResizeEvent* event)
	{
		fitInView(m_Scene->sceneRect(), Qt::KeepAspectRatio);
		m_VideoItem->setSize({ m_Scene->width(), m_Scene->height() });
	}
}