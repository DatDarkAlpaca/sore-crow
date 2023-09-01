#pragma once
#include <QFileInfo>
#include <QtWidgets/QWidget>

#include "ui_episode.h"
#include "core/project/metadata.h"
#include "utils/video_utils.h"

namespace sore
{
	class EpisodeWidget : public QWidget
	{
		Q_OBJECT

	public:
		// TODO: maybe create a lookup table for episodes.
		EpisodeWidget(QWidget* parent = nullptr, const std::string& episodeFilepath = "");

	public:
		void updateData(const std::string& episodePath);

	private:
		void setEpisodeThumbnail(const std::string& episodePath);

		void setEpisodeName(const std::string& episodePath);

	private:
		void mousePressEvent(QMouseEvent* event) override;

	public:
		inline std::string getEpisodeFilepath() const { return episodeFilepath; };

	signals:
		void episodeClicked();

	private:
		std::string episodeFilepath;
		Ui::EpisodeView ui;
	};
}