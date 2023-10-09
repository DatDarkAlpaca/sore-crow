#include "pch.h"
#include "mpv_widget.h"

namespace sore
{
	static void onRenderCallback(void* ctx)
	{
		QMetaObject::invokeMethod((MPVWidget*)ctx, "maybeUpdate");
	}

	MPVWidget::MPVWidget(QWidget* parent)
		: QOpenGLWidget(parent)
	{
		connect(&m_Handler, &MPVHandler::propertyChanged, this, &MPVWidget::propagateMPVSignal);
	}

	MPVWidget::~MPVWidget()
	{
		disconnect();
		makeCurrent();
	}

	void MPVWidget::initializeGL()
	{
		m_Handler.initializeMPVGL();

		mpv_render_context_set_update_callback(
			m_Handler.mpvContext,
			&onRenderCallback,
			reinterpret_cast<void*>(this)
		);
	}

	void MPVWidget::paintGL()
	{
		mpv_opengl_fbo mpvFBO{ static_cast<int>(
			defaultFramebufferObject()),
			(int)m_Dimensions[0],
			(int)m_Dimensions[1], 
			0
		};

		int flipY { 1 };

		mpv_render_param params[] = {
			{ MPV_RENDER_PARAM_OPENGL_FBO, &mpvFBO },
			{ MPV_RENDER_PARAM_FLIP_Y, &flipY },
			{ MPV_RENDER_PARAM_INVALID, nullptr }
		};

		// See render_gl.h on what OpenGL environment mpv expects, and
		// other API details. --> from libmpv
		mpv_render_context_render(m_Handler.mpvContext, params);
	}

	void MPVWidget::resizeGL(int width, int height)
	{
		m_Dimensions[0] = width * m_DevicePixelRatio;
		m_Dimensions[1] = height * m_DevicePixelRatio;
	}

	void MPVWidget::showEvent(QShowEvent* event)
	{
		QOpenGLWidget::showEvent(event);
		if (!m_FirstScreenShow)
			return;

		m_FirstScreenShow = false;
		
		connect(window()->windowHandle(), &QWindow::screenChanged, this, &MPVWidget::screenResized);
		screenResized(screen());
	}

	void MPVWidget::screenResized(QScreen* screen)
	{
		m_DevicePixelRatio = screen ? screen->devicePixelRatio() : 1.0;
		resizeGL(width(), height());
	}

	void MPVWidget::maybeUpdate()
	{
		if (window()->isMinimized() || !updatesEnabled())
		{
			makeCurrent();
			paintGL();
			context()->swapBuffers(context()->surface());
			reportFrameSwap();
			doneCurrent();
			return;
		}

		update();
	}

	void MPVWidget::reportFrameSwap()
	{
		if (m_Handler.mpvContext)
			mpv_render_context_report_swap(m_Handler.mpvContext);
	}

	void MPVWidget::propagateMPVSignal(const mpv_event_property& property, const QVariant& variant)
	{
		QString propertyName = property.name;
		auto propertyFormat = property.format;

		// Media Source:
		if (propertyName == "path")
			emit fileChanged(variant.toString());

		if (propertyName == "media-title")
			emit titleChanged(variant.toString());

		if (propertyName == "pause")
			emit pauseChanged(variant.toBool());

		if (propertyName == "duration")
			emit durationChanged(variant.toDouble() * 1000);

		if (propertyName == "time-pos")
			emit positionChanged(variant.toDouble() * 1000);

		if (propertyName == "fullscreen")
			emit fullscreenChanged(variant.toBool());

		if (propertyName == "track-list/count")
		{
			emit trackListChanged(variant.toLongLong());
		}

		// Video:
		if (propertyName == "vid")
		{
			if (propertyFormat == MPV_FORMAT_INT64)
				emit videoTrackChanged(variant.toInt());

			else if (propertyFormat == MPV_FORMAT_FLAG)
				emit videoDisabled();
		}

		// Audio:
		if (propertyName == "aid")
		{
			if (propertyFormat == MPV_FORMAT_INT64)
				emit audioTrackChanged(variant.toInt());

			else if (propertyFormat == MPV_FORMAT_FLAG)
				emit audioDisabled();
		}

		// Volume:
		if (propertyName == "volume")
			emit volumeChanged(variant.toInt());

		if (propertyName == "volume-max")
			emit volumeMaxChanged(variant.toInt());

		// Subtitle:
		if (propertyName == "sid")
		{
			if (propertyFormat == MPV_FORMAT_INT64)
				emit subtitleTrackChanged(variant.toInt());

			else if (propertyFormat == MPV_FORMAT_FLAG)
				emit subtitleDisabled();
		}

		if (propertyName == "secondary-sid")
		{
			if (propertyFormat == MPV_FORMAT_INT64)
				emit subtitleTwoTrackChanged(variant.toInt());

			else if (propertyFormat == MPV_FORMAT_FLAG)
				emit subtitleSecondaryDisabled();
		}

		if (propertyName == "sub-text")
		{
			if (propertyFormat != MPV_FORMAT_STRING)
				return;

			QString subtitle = *(const char**)property.data;

			if (subtitle.isEmpty())
				return;

			double start = mpv::getProperty(m_Handler.mpvHandle, "sub-start").toDouble();
			double end = mpv::getProperty(m_Handler.mpvHandle, "sub-end").toDouble();
			double delay = mpv::getProperty(m_Handler.mpvHandle, "sub-delay").toDouble();

			emit subtitleChanged(subtitle, start * 1000, end * 1000, delay * 1000);
		}

		if (propertyName == "secondary-sub-text")
		{
			if (propertyFormat != MPV_FORMAT_STRING)
				return;

			QString subtitle = *(const char**)property.data;

			if (subtitle.isEmpty())
				return;

			double start = mpv::getProperty(m_Handler.mpvHandle, "secondary-sub-start").toDouble();
			double end = mpv::getProperty(m_Handler.mpvHandle, "secondary-sub-end").toDouble();
			double delay = mpv::getProperty(m_Handler.mpvHandle, "sub-delay").toDouble();

			emit subtitleChangedSecondary(subtitle, start * 1000, end * 1000, delay * 1000);
		}

		// Audio Device:
		if (propertyName == "audio-device-list")
		{
			emit audioDevicesChanged(variant.toList().size());
		}
	}
}