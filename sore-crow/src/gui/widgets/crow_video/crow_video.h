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
		CrowVideo(QWidget* parent = nullptr);

	public:
		void setFont(const QFont& font);

		void disableSubtitleText();

		void setSubtitleText(const std::string& text);

	protected:
		void resizeEvent(QResizeEvent* event) override;

	public:
		QGraphicsVideoItem* videoItem() const { return m_VideoItem; }

	private:
		QGraphicsTextItem* m_SubtitleItem = nullptr;
		QGraphicsVideoItem* m_VideoItem = nullptr;
		QGraphicsScene* m_Scene = nullptr;
	};
}