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
			m_PrimarySubtitle = new SubtitleItem("");
			m_SecondarySubtitle = new SubtitleItem("");

			setScene(m_Scene);
		}

	public:
		void toggleSubtitles(bool enabled)
		{
			if (enabled)
			{
				m_Scene->addItem(m_PrimarySubtitle);
				m_Scene->addItem(m_SecondarySubtitle);
				return;
			}

			m_Scene->removeItem(m_PrimarySubtitle);
			m_Scene->removeItem(m_SecondarySubtitle);
		}
		
		void setPrimarySubtitle(const QString& text)
		{
			m_PrimarySubtitle->document()->setPlainText(text);
		}

		void setSecondarySubtitle(const QString& text)
		{
			m_SecondarySubtitle->document()->setPlainText(text);
		}

		void setSubtitleStyles(const SubtitleFontStyles& fontStyles)
		{
			setSubtitleStyle(m_PrimarySubtitle, fontStyles);
			setSubtitleStyle(m_SecondarySubtitle, fontStyles);		
		}

	private:
		void setSubtitleStyle(SubtitleItem* subtitle, const SubtitleFontStyles& fontStyles)
		{
			subtitle->setDefaultTextColor(fontStyles.color);
			subtitle->setFont(QFont(fontStyles.family, fontStyles.size));
			subtitle->setBackgroundColor(fontStyles.backgroundColor);
		}

	private:
		QGraphicsScene* m_Scene = nullptr;
		SubtitleItem* m_PrimarySubtitle = nullptr;
		SubtitleItem* m_SecondarySubtitle = nullptr;
	};
}