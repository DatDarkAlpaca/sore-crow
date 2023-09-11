#include "pch.h"
#include "crow_video.h"
#include "core/global_data.h"

namespace sore
{
	CrowVideo::CrowVideo(QWidget* parent)
		: QGraphicsView(parent)
	{
		m_Scene = new QGraphicsScene(parent);
		setScene(m_Scene);
		
		setup();

		resizeScene();

		setStyleSheet("border: 0;");
	}

	void CrowVideo::setFont(const QFont& font)
	{
		m_SubtitleItem->setFont(font);
	}

	void CrowVideo::setSubtitleText(const std::string& text)
	{
		m_SubtitleItem->setPlainText(text.c_str());
	}

	void CrowVideo::resizeEvent(QResizeEvent* event)
	{
		resizeScene();
	}

	void CrowVideo::setup()
	{
		m_VideoItem = new QGraphicsVideoItem;
		m_SubtitleItem = new SubtitleItem(m_VideoItem);

		// Position:
		m_VideoItem->setZValue(0);
		m_VideoItem->setPos(0, 0);

		// Subtitle:
		m_SubtitleItem->setZValue(20);
		m_SubtitleItem->setBackgroundColor(QColor(0, 0, 0, 192));
		
		// Font:
		QFont subtitleFont;
		subtitleFont.setPixelSize(DefaultSubtitlePointSize);
		
		QColor textColor = globalData.isStylesheetDark ? QColor(255, 255, 255, 255) : QColor(0, 0, 0, 255);
		m_SubtitleItem->setDefaultTextColor(textColor);
		m_SubtitleItem->setFont(subtitleFont);

		// Adding:
		m_Scene->addItem(m_SubtitleItem);
		m_Scene->addItem(m_VideoItem);		
	}

	void CrowVideo::resizeScene()
	{
		m_Scene->setSceneRect(0, 0, (float)width(), (float)height());
		m_VideoItem->setSize({ (float)width(), (float)height() });

		m_SubtitleItem->setX(m_VideoItem->boundingRect().center().x() - m_SubtitleItem->boundingRect().width() / 2);
		m_SubtitleItem->setY(m_VideoItem->boundingRect().bottom() - m_SubtitleItem->boundingRect().height());
	}
}