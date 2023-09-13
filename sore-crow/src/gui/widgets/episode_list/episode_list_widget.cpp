#include "pch.h"
#include "episode_list_widget.h"

namespace sore
{
    EpisodeListWidget::EpisodeListWidget(QWidget* parent)
        : QListWidget(parent)
    {
    }

    void EpisodeListWidget::insertProjectEpisodes(const ProjectData& data)
	{
        namespace fs = std::filesystem;

        size_t index = 0;
        for (const auto& episode : data.mediaData.episodeData)
        {
            EpisodeWidget* episodeWidget = new EpisodeWidget(this);
            episodeWidget->updateData(episode.filepath);

            m_EpisodeSourceLookup.push_back({ index, episode.filepath });

            onEpisodeClicked(episodeWidget, episode.filepath);
            addEpisodeToList(episodeWidget);

            ++index;
        }
	}

    void EpisodeListWidget::addEpisodeToList(EpisodeWidget* episodeWidget)
    {
        QListWidgetItem* item = new QListWidgetItem(this);

        item->setSizeHint(episodeWidget->sizeHint());

        addItem(item);
        setItemWidget(item, episodeWidget);
    }

    void EpisodeListWidget::onEpisodeClicked(EpisodeWidget* episodeWidget, const std::string& episodeFilepath)
    {
        QObject::connect(episodeWidget, &EpisodeWidget::episodeClicked, [&, episodeFilepath]() {
            auto index = getIndexFromSource(episodeFilepath);
            if (!index.has_value())
                return;

            m_CurrenEpisodeIndex = index.value();
            emit episodeFromListClicked(episodeFilepath);
        });
    }

    std::optional<int> EpisodeListWidget::setPreviousEpisode()
    {
        QListWidgetItem* itemWidget = currentItem();
        if (!itemWidget)
            return std::nullopt;

        int currentIndex = row(itemWidget);
        if (currentIndex == 0)
            return std::nullopt;

        int nextIndex = currentIndex - 1;
        QListWidgetItem* next = item(nextIndex);
        if (!next)
            return std::nullopt;

        setCurrentItem(next);
        m_CurrenEpisodeIndex = nextIndex;

        return m_CurrenEpisodeIndex;
    }

    std::optional<int> EpisodeListWidget::setNextEpisode()
    {
        QListWidgetItem* itemWidget = currentItem();
        if (!itemWidget)
            return std::nullopt;

        int currentIndex = row(itemWidget);
        if (currentIndex == count() - 1)
            return std::nullopt;

        int nextIndex = currentIndex + 1;
        QListWidgetItem* next = item(nextIndex);
        if (!next)
            return std::nullopt;

        setCurrentItem(next);
        m_CurrenEpisodeIndex = nextIndex;

        return m_CurrenEpisodeIndex;
    }

    std::optional<int> EpisodeListWidget::getIndexFromSource(const std::string& episodeSource)
    {
        for (const auto& [index, source] : m_EpisodeSourceLookup)
        {
            if (source == episodeSource)
                return index;
        }

        return std::nullopt;
    }

    std::optional<std::string> EpisodeListWidget::getSourceFromIndex(int index)
    {
        for (const auto& [i, source] : m_EpisodeSourceLookup)
        {
            if (i == index)
                return source;
        }

        return std::nullopt;
    }
}