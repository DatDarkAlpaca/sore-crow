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
		EpisodeManager(QWidget* parent = nullptr)
			: QWidget(parent)
		{
			ui.setupUi(this);

			connect(ui.addEpisodeBtn, &QPushButton::released, this, &EpisodeManager::onAddEpisodeClicked);
			connect(ui.removeEpisodeBtn, &QPushButton::released, this, &EpisodeManager::onRemoveEpisodeClicked);
		}

	public:
		void onAddEpisodeClicked()
		{
			auto episodesSelected = openMediaSourcesDialog();
			emit episodesAddClicked(episodesSelected);
		}

		void onRemoveEpisodeClicked()
		{
			int reply = inquiryBox("SoreCrow", "Do you really want to remove this media file from the project?");
			if (reply == QMessageBox::Yes)
				emit episodeRemoveClicked();
		}

	signals:
		void episodesAddClicked(const QStringList&);

		void episodeRemoveClicked();

	private:
		Ui::EpisodeManager ui;
	};
}