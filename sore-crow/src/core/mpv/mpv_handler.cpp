#include "pch.h"
#include "mpv_handler.h"
#include "utils/message_box_utils.h"

static void* getProcAddress(void* ctx, const char* name)
{
	QOpenGLContext* context = QOpenGLContext::currentContext();

	if (!context)
		return nullptr;

	return reinterpret_cast<void*>(context->getProcAddress(QByteArray(name)));
}

static void wakeup(void* ctx)
{
	QMetaObject::invokeMethod((sore::MPVHandler*)ctx, "onMPVEvents", Qt::QueuedConnection);
}

namespace sore
{
	MPVHandler::MPVHandler(QObject* parent)
		: QObject(parent)
	{
		initializeMPV();
		setupObservers();

		mpv_set_wakeup_callback(mpvHandle, wakeup, this);
	}

	MPVHandler::~MPVHandler()
	{
		if (mpvContext)
			mpv_render_context_free(mpvContext);

		mpv_terminate_destroy(mpvHandle);
	}

	void MPVHandler::initializeMPVGL()
	{
		mpv_opengl_init_params gl_init_params[1] = { getProcAddress, nullptr };

#if MPV_CLIENT_API_VERSION < MPV_MAKE_VERSION(2, 0)
		gl_init_params.extra_exts = nullptr;
#endif

		mpv_render_param display{ MPV_RENDER_PARAM_INVALID, nullptr };

		mpv_render_param params[]
		{
			{ MPV_RENDER_PARAM_API_TYPE, const_cast<char*>(MPV_RENDER_API_TYPE_OPENGL) },
			{ MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &gl_init_params },
			display,
			{ MPV_RENDER_PARAM_INVALID, nullptr }
		};

		if (mpv_render_context_create(&mpvContext, mpvHandle, params) < 0)
		{
			errorBox("Crow Player [MPV]", "Failed to initialize a valid OpenGL Context for mpv");
			return;
		}
	}

	void MPVHandler::setupObservers()
	{
		observeProperty("path", MPV_FORMAT_STRING);

		observeProperty("media-title", MPV_FORMAT_STRING);
		observeProperty("pause", MPV_FORMAT_FLAG);
		observeProperty("duration", MPV_FORMAT_DOUBLE);
		observeProperty("time-pos", MPV_FORMAT_DOUBLE);
		observeProperty("fullscreen", MPV_FORMAT_FLAG);
		observeProperty("track-list/count", MPV_FORMAT_INT64);

		observeProperty("vid", MPV_FORMAT_INT64); // Audio ID
		observeProperty("aid", MPV_FORMAT_INT64); // Audio ID

		observeProperty("volume", MPV_FORMAT_INT64);
		observeProperty("volume-max", MPV_FORMAT_INT64);

		observeProperty("sid", MPV_FORMAT_INT64);					// Subtitle ID
		observeProperty("secondary-sid", MPV_FORMAT_INT64);			// Secondary Subtitle ID
		observeProperty("sub-text", MPV_FORMAT_STRING);				// Secondary Subtitle ID
		observeProperty("secondary-sub-text", MPV_FORMAT_STRING);	// Secondary Subtitle ID

		observeProperty("audio-device-list", MPV_FORMAT_NODE);
	}

	void MPVHandler::initializeMPV()
	{
		mpvHandle = mpv_create();
		if (!mpvHandle)
		{
			errorBox("Crow Player [MPV]", "Failed to create a valid mpv handler");
			return;
		}

		mpv_set_option_string(mpvHandle, "terminal", "yes");
		mpv_set_option_string(mpvHandle, "keep-open", "yes");
		mpv_set_option_string(mpvHandle, "force-seekable", "yes");
		mpv_set_option_string(mpvHandle, "msg-level", "all=no");
		mpv_set_option_string(mpvHandle, "ytdl", "no");

		if (mpv_initialize(mpvHandle) < 0)
		{
			errorBox("Crow Player [MPV]", "Failed to initialize the mpv handler");
			return;
		}

		mpv::setOption(mpvHandle, "hwdec", "auto");
	}

	void MPVHandler::handleMPVevent(mpv_event* event)
	{
		switch (event->event_id)
		{
			case MPV_EVENT_FILE_LOADED:
			{
				int64_t width = mpv::getProperty(mpvHandle, "video-params/w").toInt();
				int64_t height = mpv::getProperty(mpvHandle, "video-params/h").toInt();
				emit fileLoaded(width, height);
			} break;

			case MPV_EVENT_PROPERTY_CHANGE:
			{
				mpv_event_property* property = (mpv_event_property*)event->data;
				QVariant data = mpv::propertyToVariant(property);
				emit propertyChanged(*property, data);
			} break;
		}
	}

	void MPVHandler::observeProperty(const char* propertyName, mpv_format format)
	{
		mpv_observe_property(mpvHandle, 0, propertyName, format);
	}

	void MPVHandler::onMPVEvents()
	{
		while (mpvHandle)
		{
			mpv_event* event = mpv_wait_event(mpvHandle, 0);

			if (event->event_id == MPV_EVENT_NONE)
				break;

			handleMPVevent(event);
		}
	}
}