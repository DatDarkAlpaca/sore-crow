#pragma once
#include <mpv/client.h>
#include <QOpenGLContext>
#include <mpv/render_gl.h>
#include "utils/mpv_node.h"
#include "utils/mpv_commands.h"

namespace sore
{
	class MPVHandler : public QObject
	{
		Q_OBJECT

	public:
		MPVHandler(QObject* parent = nullptr);

		~MPVHandler();

	public:
		void initializeMPVGL();

		void setupObservers();

	private slots:
		void onMPVEvents();

	private:
		void initializeMPV();

		void handleMPVevent(mpv_event* event);

		void observeProperty(const char* propertyName, mpv_format format);

	signals:
		void propertyChanged(const mpv_event_property& property, const QVariant& variant);

		void fileLoaded(int64_t width, int64_t height);

	public:
		mpv_handle* mpvHandle = nullptr;
		mpv_render_context* mpvContext = nullptr;
	};
}