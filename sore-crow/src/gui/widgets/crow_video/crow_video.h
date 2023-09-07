#pragma once
#include <QGraphicsView>
#include <QGraphicsVideoItem>
#include "gui/objects/subtitle_item.h"

namespace sore
{
	class CrowVideo : public QGraphicsView
	{
		Q_OBJECT

	public:
		CrowVideo(QWidget* parent = nullptr);

	public:
		void setFont(const QFont& font);

		void setSubtitleText(const std::string& text);

	protected:
		void resizeEvent(QResizeEvent* event) override;

	public:
		QGraphicsVideoItem* videoItem() const { return m_VideoItem; }

		bool enabledSubtitles() const { return m_EnabledSubtitles; }

		void setEnabledSubtitles(bool value) { m_EnabledSubtitles = value; }

	public:
		void resizeScene();

	private:
		void setup();

	private:
		SubtitleItem* m_SubtitleItem = nullptr;
		QGraphicsVideoItem* m_VideoItem = nullptr;
		QGraphicsScene* m_Scene = nullptr;

	private:
		bool m_EnabledSubtitles = false;
	};
}