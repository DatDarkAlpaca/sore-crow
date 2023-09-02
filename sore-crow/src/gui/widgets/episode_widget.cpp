#include "pch.h"
#include "episode_widget.h"

namespace sore
{
	EpisodeWidget::EpisodeWidget(QWidget* parent)
		: QWidget(parent)
	{
		ui.setupUi(this);
	}

	void EpisodeWidget::updateData(const std::string& episodePath)
	{
		setEpisodeThumbnail(episodePath);

		setEpisodeName(episodePath);
	}

	void EpisodeWidget::setEpisodeThumbnail(const std::string& episodePath)
	{
		// TODO: implement	
	}

	void EpisodeWidget::setEpisodeName(const std::string& episodePath)
	{
		QFileInfo fileInfo(QString::fromStdString(episodePath));
		ui.name->setText(fileInfo.fileName());
	}

	void EpisodeWidget::mousePressEvent(QMouseEvent* event)
	{
		if (event->button() == Qt::LeftButton)
			emit episodeClicked();

		QWidget::mousePressEvent(event);
	}
}