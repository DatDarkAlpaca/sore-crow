#pragma once
#include "ui_episode_manager.h"
#include "utils/dialog_utils.h"
#include "utils/message_box_utils.h"

namespace sore
{
	class EpisodeManager : public QWidget
	{
		Q_OBJECT

	public:
		EpisodeManager(QWidget* parent = nullptr);

	public:
		void onAddEpisodeClicked();

		void onRemoveEpisodeClicked();

	signals:
		void episodesAddClicked(const QStringList&);

		void episodeRemoveClicked();

	private:
		Ui::EpisodeManager ui;
	};
}