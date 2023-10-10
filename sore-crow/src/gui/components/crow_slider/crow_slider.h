#pragma once
#include <QSlider>

namespace sore
{
	class CrowSlider : public QSlider
	{
	public:
		CrowSlider(QWidget* parent = nullptr)
			: QSlider(parent)
		{
			connect(this, &QSlider::sliderPressed, this, &CrowSlider::setDragged);
			connect(this, &QSlider::sliderReleased, this, &CrowSlider::setUndragged);
		}

	private slots:
		void setDragged() { m_IsBeingDragged = true; }

		void setUndragged() { m_IsBeingDragged = false; }

	public:
		bool isBeingDragged() const { return m_IsBeingDragged; }

	private:
		bool m_IsBeingDragged = false;
	};
}