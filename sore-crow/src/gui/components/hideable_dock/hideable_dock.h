#pragma once
#include <QDockWidget>

namespace sore
{
	class HideableDockWidget : public QDockWidget
	{
		Q_OBJECT

	public:
		HideableDockWidget(QWidget* parent = nullptr)
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