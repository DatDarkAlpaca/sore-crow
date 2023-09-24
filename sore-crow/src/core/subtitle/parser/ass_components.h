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
		int fontSize;
		QColor primaryColor;
		QColor secondaryColor;
		QColor outlineColor;
		QColor backgroundColor;

		bool bold, italic, underline, strikeout;
		double scaleX, scaleY;
		int64_t spacing, angle;

		BorderStyle borderStyle = BorderStyle::NONE;
		short outline, shadow;
		int alignment;

		double marginL, marginR, marginV;
		double alphaLevel;
		int encoding;
	};

	struct Dialogue
	{
		bool marked;
		int layer;
		double startMs, endMs;
		QString styleName;
		QString characterName;

		short marginL, marginR, marginV;
		bool effect; // not used.

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