#include "pch.h"
#include "episode_manager.h"

namespace sore
{
	EpisodeManager::EpisodeManager(QWidget* parent)
		: QWidget(parent)
	{
		ui.setupUi(this);

		connect(ui.addEpisodeBtn, &QPushButton::released, this, &EpisodeManager::onAddEpisodeClicked);
		connect(ui.removeEpisodeBtn, &QPushButton::released, this, &EpisodeManager::onRemoveEpisodeClicked);
	}

	void EpisodeManager::onAddEpisodeClicked()
	{
		auto episodesSelected = openMediaSourcesDialog();
		emit episodesAddClicked(episodesSelected);
	}

	void EpisodeManager::onRemoveEpisodeClicked()
	{
		int reply = inquiryBox("SoreCrow", "Do you really want to remove this media file from the project?");
		if (reply == QMessageBox::Yes)
			emit episodeRemoveClicked();
	}
}