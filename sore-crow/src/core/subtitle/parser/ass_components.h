#pragma once
#include <QString>
#include <QColor>
#include "core/subtitle/subtitles.h"

namespace sore::ass
{
	enum BorderStyle { NONE = 0, BORDER_OUTLINE_DROP_SHADOW = 1, BORDER_OPAQUE = 3 };

	struct SubtitleStyle
	{
		QString styleName;
		QString fontName;
		int fontSize = 12;
		QColor primaryColor;
		QColor secondaryColor;
		QColor outlineColor;
		QColor backgroundColor;

		bool bold = false, italic = false, underline = false, strikeout = false;
		double scaleX = 1, scaleY = 1;
		int64_t spacing = 0, angle = 0;

		BorderStyle borderStyle = BorderStyle::NONE;
		short outline = 0, shadow = 0;
		int alignment = 0;

		double marginL = 0, marginR = 0, marginV = 0;
		double alphaLevel = 0;
		int encoding = 0;
	};

	struct Dialogue
	{
		bool marked = false;
		int layer = 0;
		double startMs = 0, endMs = 0;
		QString styleName;
		QString characterName;

		short marginL = 0, marginR = 0, marginV = 0;
		//bool effect = false; // not used.

		QString text;
	};

	struct Subtitles : ISubtitles
	{
	public:
		Subtitles()
			: ISubtitles(SubtitleType::ASS)
		{

		}

	public:
		std::vector<SubtitleStyle> styles;
		std::vector<Dialogue> dialogues;
	};
}