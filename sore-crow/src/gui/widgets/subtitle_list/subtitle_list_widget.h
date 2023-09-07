#pragma once
#include <QListWidget>

#include "gui/widgets/episode_widget/episode_widget.h"
#include "core/subtitle/subtitle_data.h"

namespace sore
{
	class SubtitleListWidget : public QListWidget
	{
		Q_OBJECT

	public:
		SubtitleListWidget(QWidget* parent = nullptr)
		{

		}

	public:
		void insertSubtitleData(const std::vector<SubtitleData>& subtitleData)
		{
			for (const auto& subtitle : subtitleData)
			{

			}

			/*using namespace std::filesystem;

			for (size_t i = 0; i < episodes.size(); ++i)
			{
				path episodePath = root / episodeFolder / path(episodes[i].filename);

				EpisodeWidget* episodeWidget = new EpisodeWidget(this);
				episodeWidget->updateData(episodePath.string());


				onEpisodeClicked(episodeWidget, episodePath.string());
				addEpisodeToList(episodeWidget);
			}*/
		}

	private:
		void addEpisodeToList(EpisodeWidget* widget)
		{

		}

		void onEpisodeClicked(EpisodeWidget* episodeWidget, const std::string& episodeFilepath) 
		{

		}

	signals:
		void subtitleFromListClicked(const std::string&);
	};
}