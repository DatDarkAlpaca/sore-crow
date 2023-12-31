#pragma once
#include <string>
#include <QSettings>

namespace sore
{
	class CrowSettings : public QSettings
	{
	public:
		CrowSettings(const QString& fileName, Format format, QObject* parent = nullptr)
			: QSettings(fileName, format, parent)
		{
		}

	public:
		QString getString(const QString& entry) const
		{
			return value(entry).toString();
		}

		QStringList getStrings(const QString& entry) const
		{
			return value(entry).toString().split(",");
		}

		std::string getStdString(const QString& entry) const
		{
			return value(entry).toString().toStdString();
		}
	};
}