#pragma once
#include <QJsonValue>
#include <QJsonObject>

#include "plugin_type.h"
#include "core/handlers.h"
#include "utils/message_box_utils.h"

namespace sore
{
	struct ManifestSources
	{
		QString mainSource;
		std::vector<QString> dependencies = {};
	};

	static inline ManifestSources parseSources(const QJsonObject& object, const QString& pluginFolder)
	{
		ManifestSources sources;

		if (const QJsonValue value = object["main"]; value.isString())
		{
			QString mainFilepath = pluginFolder + QDir::separator() + value.toString();
			sources.mainSource = mainFilepath;
		}

		if (const QJsonValue value = object["dependencies"]; value.isArray())
		{
			for (const auto& dependency : value.toArray()) {
				QString folder = pluginFolder + QDir::separator() + dependency.toString();
				sources.dependencies.push_back(folder);
			}
		}

		return sources;
	}

	class Manifest
	{
	public:
		Manifest(const QString& filepath)
			: filepath(filepath)
		{
		}

		Manifest() = default;

	public:
		void initialize()
		{
			QFileInfo path(filepath);
			auto object = FilesystemHandler::loadFromJsonFile(path.canonicalFilePath());

			if (const QJsonValue value = object["version"]; value.isString())
				version = value.toString();

			if (const QJsonValue value = object["plugin-name"]; value.isString())
				pluginName = value.toString();

			if (const QJsonValue value = object["plugin-type"]; value.isString())
			{
				pluginType = stringToPluginTYpe(value.toString());

				if(pluginType == PluginType::NONE)
				{
					QString message = QString("Invalid plugin type: %1").arg(value.toString());
					throw std::exception(message.toStdString().c_str());
				}
			}

			if (const QJsonValue value = object["author"]; value.isString())
				author = value.toString();

			if (const QJsonValue value = object["url"]; value.isString())
				url = value.toString();

			if (const QJsonValue value = object["sources"]; value.isObject())
				sources = parseSources(value.toObject(), path.absolutePath());
		}

	public:
		QString filepath;
		QString version;
		QString pluginName;
		PluginType pluginType = PluginType::NONE;
		QString author;
		QString url;
		ManifestSources sources = {};
	};
}