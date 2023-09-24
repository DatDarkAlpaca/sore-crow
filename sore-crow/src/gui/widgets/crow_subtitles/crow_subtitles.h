#pragma once
#include <QWidget>
#include <QTextEdit>

namespace sore
{
	class CrowSubtitles : public QWidget
	{
		Q_OBJECT

	public:
		CrowSubtitles(QWidget* parent = nullptr)
			: QWidget(parent)
		{
			m_TextEdit.setText("hello");
		}

	private:
		QTextEdit m_TextEdit;
	};
}