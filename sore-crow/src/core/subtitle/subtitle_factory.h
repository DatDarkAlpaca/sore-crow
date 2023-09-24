#pragma once
#include <QFileInfo>
#include <memory>
#include "parser/srt_parser.h"
#include "parser/ass_parser.h"

namespace sore
{
	class SubtitleFactory
	{
	public:
		SubtitleFactory(const QString& filepath)
			: m_Filepath(filepath)
		{
			
		}

	public:
		std::unique_ptr<ISubtitles> parse()
		{
			QFile file(m_Filepath);
			file.open(QFile::ReadOnly);

			QTextStream textStream(&file);

			QFileInfo info(file);
			if (info.suffix().toLower() == "srt")
				return std::make_unique<srt::Subtitles>(parseSRT(textStream));

			else if (info.suffix().toLower() == "ass")
				return std::make_unique<ass::Subtitles>(parseASS(textStream));

			return nullptr;
		}

	private:
		QString m_Filepath;
	};
}