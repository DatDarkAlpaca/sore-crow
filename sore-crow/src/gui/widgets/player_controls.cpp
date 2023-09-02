#include "pch.h"
#include "player_controls.h"
#include "utils/string_utils.h"

namespace sore
{
	PlayerControlsWidget::PlayerControlsWidget(QWidget* parent)
		: QWidget(parent)
	{
		ui.setupUi(this);

		setupIcons();
		setupAudioSlider();

		onPlayerSliderPositionChanged();
	}

	void PlayerControlsWidget::togglePlayButtonIcon(bool togglePlayingIcon)
	{
		// TODO: relative paths
		if (togglePlayingIcon)
			ui.playVideoBtn->setIcon(QIcon("C:/Users/paulo/Projects/sore-crow/sore-crow/resources/icons/play.png"));
		else
			ui.playVideoBtn->setIcon(QIcon("C:/Users/paulo/Projects/sore-crow/sore-crow/resources/icons/pause.png"));
	}

	void PlayerControlsWidget::setVideoSliderPosition(long long position)
	{
		ui.playerSlider->setValue(position);
	}

	void PlayerControlsWidget::setVideoSliderMaximum(long long maximum)
	{
		ui.playerSlider->setMaximum(maximum);
	}

	void PlayerControlsWidget::blockPlayerSliderSignals(bool value)
	{
		ui.playerSlider->blockSignals(value);
	}

	void PlayerControlsWidget::onPlayerSliderPositionChanged()
	{
		QObject::connect(ui.playerSlider, &QSlider::valueChanged, [&](long long position) {
			setCurrentDurationLabel(position);
		});
	}

	void PlayerControlsWidget::setCurrentDurationLabel(long long duration)
	{
		ui.currentPositionLabel->setText(getDurationString(duration).c_str());
	}

	void PlayerControlsWidget::setTotalDurationLabel(long long duration)
	{
		ui.totalDurationLabel->setText(getDurationString(duration).c_str());
	}

	void PlayerControlsWidget::setupAudioSlider()
	{
		ui.volumeSlider->setMaximum(100);
		ui.volumeSlider->setMinimum(0);
		ui.volumeSlider->setValue(50);
	}

	void PlayerControlsWidget::setupIcons()
	{
		ui.playVideoBtn->setText("");
		ui.stopVideoBtn->setText("");
		ui.previousBtn->setText("");
		ui.nextBtn->setText("");

		// TODO: set relative paths when the file system is ready
		ui.playVideoBtn->setIcon(QIcon("C:/Users/paulo/Projects/sore-crow/sore-crow/resources/icons/play.png"));
		ui.stopVideoBtn->setIcon(QIcon("C:/Users/paulo/Projects/sore-crow/sore-crow/resources/icons/stop.png"));
		ui.previousBtn->setIcon(QIcon("C:/Users/paulo/Projects/sore-crow/sore-crow/resources/icons/previous.png"));
		ui.nextBtn->setIcon(QIcon("C:/Users/paulo/Projects/sore-crow/sore-crow/resources/icons/next.png"));
	}
}