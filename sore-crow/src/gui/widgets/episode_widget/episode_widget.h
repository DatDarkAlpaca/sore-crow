#pragma once
#include <QFileInfo>
#include <QtWidgets/QWidget>

#include "ui_episode.h"
#include "utils/video_utils.h"

namespace sore
{
	class EpisodeWidget : public QWidget
	{
		Q_OBJECT

	public:
		EpisodeWidget(QWidget* parent = nullptr);

	public:
		void updateData(const std::string& episodePath);

	private:
		void setEpisodeThumbnail(const std::string& episodePath);

		void setEpisodeName(const std::string& episodePath);

	private:
		void mousePressEvent(QMouseEvent* event) override;

	signals:
		void episodeClicked();

	private:
		Ui::EpisodeView ui;
	};
}