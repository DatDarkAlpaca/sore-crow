#pragma once
#include <QPainter>
#include <QStyledItemDelegate>
#include "subtitle_model.h"

namespace sore
{
	class SubtitleDelegate : public QStyledItemDelegate
	{
	public:
		SubtitleDelegate(QObject* parent = nullptr)
			: QStyledItemDelegate(parent) 
		{
		}

	public:
        void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& modelIndex) const override
        {
            if (option.state & QStyle::State_Selected)
                painter->fillRect(option.rect, option.palette.highlight());

            QString subtitleText = modelIndex.data(SubtitleModel::Roles::TextRole).toString();
            SubtitleType subtitleType = (SubtitleType)modelIndex.data(SubtitleModel::Roles::TypeRole).toInt();
            
            if (subtitleType == SubtitleType::ASS)
                modifyText(subtitleText);

            painter->save();
            painter->setPen(option.palette.text().color());
            painter->drawText(option.rect, Qt::AlignVCenter, subtitleText);
            painter->restore();
        }

    private:
        QString modifyText(QString& originalText) const
        {
            QRegularExpression regex("\\{\\\\[^}]*\\}");
            return originalText.replace(regex, "");
        }
	};
}