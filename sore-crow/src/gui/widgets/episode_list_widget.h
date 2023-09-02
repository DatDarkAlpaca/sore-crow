#pragma once
#include <QListWidget>
#include <optional>

#include "gui/widgets/episode_widget.h"
#include "core/project/project_data.h"

namespace sore
{
	class EpisodeListWidget : public QListWidget
	{
		Q_OBJECT

	public:
		EpisodeListWidget(QWidget* parent = nullptr);

	public:
		void insertProjectEpisodes(const ProjectData& data);

		std::optional<int> setPreviousEpisode();

		std::optional<int> setNextEpisode();

	private:
		void addEpisodeToList(EpisodeWidget* widget);

		void onEpisodeClicked(EpisodeWidget* episodeWidget, const std::string& episodeFilepath);

	public:
		std::optional<int> getIndexFromSource(const std::string& episodeSource);

		std::optional<std::string> getSourceFromIndex(int index);

	signals:
		void episodeFromListClicked(const std::string&);

	private:
		std::vector<std::pair<int, std::string>> m_EpisodeSourceLookup;
		int m_CurrenEpisodeIndex = 0;
	};
}