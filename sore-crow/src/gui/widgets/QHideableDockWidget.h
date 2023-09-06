#pragma once
#include <QDockWidget>

namespace sore
{
	class QHideableDockWidget : public QDockWidget
	{
		Q_OBJECT

	public:
		QHideableDockWidget(QWidget* parent = nullptr)
			: QDockWidget(parent)
		{
			
		}

	protected:
		void closeEvent(QCloseEvent* event)
		{
			hide();
		}
	};
}