#pragma once
#include <array>
#include <QOpenGLWidget>
#include "core/mpv/mpv_handler.h"
#include "core/mpv/utils/mpv_tracks.h"

namespace sore
{
	class MPVWidget : public QOpenGLWidget
	{
		Q_OBJECT

	public:
		MPVWidget(QWidget* parent = nullptr);

		~MPVWidget();

	protected:
		void initializeGL() override;

		void paintGL() override;

		void resizeGL(int width, int height) override;

		void showEvent(QShowEvent* event) override;

	private slots:
		void screenResized(QScreen* screen);

		void maybeUpdate();

		void reportFrameSwap();

		void propagateMPVSignal(const mpv_event_property& property, const QVariant& variant);

	signals:
		// File:
		void mediaOpened();

		// Media Source:
		void fileChanged(QString path);

		void titleChanged(QString name);

		void pauseChanged(bool paused);

		void durationChanged(double value);

		void positionChanged(double value);

		void fullscreenChanged(bool full);

		void trackListChanged(long long trackAmount);

		// Video:
		void videoTrackChanged(long long id);

		void videoDisabled();

		// Audio:
		void audioTrackChanged(long long id);

		void audioDisabled();

		// Volume:
		void volumeChanged(long long value);

		void volumeMaxChanged(long long value);

		// Subtitle:
		void subtitleTrackChanged(long long id);

		void subtitleTwoTrackChanged(long long id);

		void subtitleDisabled();

		void subtitleSecondaryDisabled();

		void subtitleChanged(const QString& subtitle, double start, double end, double delay);

		void subtitleChangedSecondary(const QString& subtitle, double start, double end, double delay);

		// Audio Devices:
		void audioDevicesChanged(int64_t deviceAmount);

	public:
		inline mpv_handle* mpvHandle() const { return m_Handler.mpvHandle; }

		inline mpv_render_context* mpvContext() const { return m_Handler.mpvContext; }

	private:
		std::array<int, 2> m_Dimensions = {};
		double m_DevicePixelRatio = 1.0;
		bool m_FirstScreenShow = true;

	private:
		MPVHandler m_Handler;
	};
}