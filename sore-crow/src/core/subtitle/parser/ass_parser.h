#pragma once
#include <vector>
#include <QString>
#include <QStringList>

#include "ass_components.h"
#include "utils/string_utils.h"

namespace
{
	static inline QColor parseASSColor(const QString& color)
	{
		QString colorString = color.mid(2);

		bool ok;
		int alpha = colorString.mid(0, 2).toInt(&ok, 16);
		int red = colorString.mid(2, 2).toInt(&ok, 16);
		int green = colorString.mid(4, 2).toInt(&ok, 16);
		int blue = colorString.mid(6, 2).toInt(&ok, 16);

		if (!ok)
		{
			// TODO: error
			return {};
		}

		return QColor(red, green, blue, alpha);
	}

	static inline bool parseASSBool(const QString& boolean)
	{
		return boolean == "-1";
	}
}

namespace sore
{
	enum class SectionASS { NONE = 0, SCRIPT_INFO, STYLES, EVENTS };

	static inline QStringList parseFormat(QString& formatLine)
	{
		formatLine.remove(0, 6 + 1); // Removes "Format:"
		return formatLine.simplified().replace(" ", "").split(",");
	}

	static inline ass::SubtitleStyle parseStyle(QString& styleLine, const QStringList& formatList)
	{
		styleLine.remove(0, 5 + 1); // Removes "Style:"
		styleLine = styleLine.simplified().replace(" ", "");

		QStringList styleElements = styleLine.split(",");

		ass::SubtitleStyle style;
		for (size_t i = 0; i < formatList.size(); ++i)
		{
			if (formatList[i] == "Name")
				style.styleName = styleElements[i];
			else if (formatList[i] == "Fontname")
				style.fontName = styleElements[i];
			else if (formatList[i] == "Fontsize")
				style.fontSize = styleElements[i].toInt();

			else if (formatList[i] == "PrimaryColour")
				style.primaryColor = parseASSColor(styleElements[i]);
			else if (formatList[i] == "SecondaryColour")
				style.secondaryColor = parseASSColor(styleElements[i]);
			else if (formatList[i] == "OutlineColor")
				style.outlineColor = parseASSColor(styleElements[i]);
			else if (formatList[i] == "BackColour")
				style.outlineColor = parseASSColor(styleElements[i]);

			else if (formatList[i] == "Bold")
				style.bold = parseASSBool(styleElements[i]);
			else if (formatList[i] == "Italic")
				style.italic = parseASSBool(styleElements[i]);
			else if (formatList[i] == "Underline")
				style.underline = parseASSBool(styleElements[i]);
			else if (formatList[i] == "Strikeout")
				style.strikeout = parseASSBool(styleElements[i]);

			else if (formatList[i] == "ScaleX")
				style.scaleX = styleElements[i].toDouble();
			else if (formatList[i] == "ScaleY")
				style.scaleY = styleElements[i].toDouble();
			else if (formatList[i] == "Spacing")
				style.spacing = styleElements[i].toInt();
			else if (formatList[i] == "Angle")
				style.angle = styleElements[i].toInt();

			else if (formatList[i] == "BorderStyle")
				style.borderStyle = (ass::BorderStyle)styleElements[i].toInt();
			else if (formatList[i] == "Outline")
				style.outline = styleElements[i].toInt();
			else if (formatList[i] == "Shadow")
				style.shadow = styleElements[i].toInt();

			else if (formatList[i] == "Alignment")
				style.shadow = styleElements[i].toInt();

			else if (formatList[i] == "MarginL")
				style.marginL = styleElements[i].toInt();
			else if (formatList[i] == "MarginR")
				style.marginR = styleElements[i].toInt();
			else if (formatList[i] == "MarginV")
				style.marginV = styleElements[i].toInt();

			else if (formatList[i] == "AlphaLevel")
				style.alphaLevel = styleElements[i].toInt();
			else if (formatList[i] == "Encoding")
				style.encoding = styleElements[i].toInt();
		}

		return style;
	}

	static inline ass::Dialogue parseDialogue(QString& dialogueLine, const QStringList& formatList)
	{
		dialogueLine.remove(0, 8 + 1); // Removes "Dialogue:"
		dialogueLine = dialogueLine.simplified().replace(" ", "");

		QStringList dialogueElements = dialogueLine.split(",");

		ass::Dialogue dialogue;
		for (size_t i = 0; i < formatList.size(); ++i)
		{
			if (formatList[i] == "Marked")
				dialogue.marked = parseASSBool(dialogueElements[i]);

			else if (formatList[i] == "Layer")
				dialogue.layer = dialogueElements[i].toInt();

			else if (formatList[i] == "Start")
				dialogue.startMs = timeToMilliseconds(dialogueElements[i]);
			else if (formatList[i] == "End")
				dialogue.endMs = timeToMilliseconds(dialogueElements[i]);

			else if (formatList[i] == "Style")
				dialogue.styleName = dialogueElements[i];
			else if (formatList[i] == "Name")
				dialogue.characterName = dialogueElements[i];

			else if (formatList[i] == "MarginL")
				dialogue.marginL = dialogueElements[i].toInt();
			else if (formatList[i] == "MarginR")
				dialogue.marginR = dialogueElements[i].toInt();
			else if (formatList[i] == "MarginV")
				dialogue.marginV = dialogueElements[i].toInt();

			else if (formatList[i] == "Effect")
				continue;

			else if (formatList[i] == "Text")
				dialogue.text = dialogueElements[i];
		}

		return dialogue;
	}

	inline ass::Subtitles parseASS(QTextStream& textStream)
	{
		std::vector<ass::SubtitleStyle> styles;
		std::vector<ass::Dialogue> dialogues;
		QStringList styleFormat, eventFormat;

		SectionASS currentSection = SectionASS::NONE;
		while (!textStream.atEnd())
		{
			QString line = textStream.readLine().trimmed();

			if (line.isEmpty())
				continue;

			// Determine Section:
			if (line == "[Script Info]")
			{
				currentSection = SectionASS::SCRIPT_INFO;
				continue;
			}
			
			else if (line == "[V4+ Styles]")
			{
				currentSection = SectionASS::STYLES;
				continue;
			}

			else if (line == "[Events]")
			{
				currentSection = SectionASS::EVENTS;
				continue;
			}

			// Parse Section:
			switch (currentSection)
			{
				case SectionASS::NONE:
					break;

				case SectionASS::SCRIPT_INFO:
					break;

				case SectionASS::STYLES:
				{
					if (line.startsWith("Format:"))
						styleFormat = parseFormat(line);
					
					else if (line.startsWith("Style:"))
						styles.push_back(parseStyle(line, styleFormat));
				} break;

				case SectionASS::EVENTS:
				{
					if (line.startsWith("Format:"))
						eventFormat = parseFormat(line);

					else if (line.startsWith("Dialogue:"))
						dialogues.push_back(parseDialogue(line, eventFormat));
				} break;
			}
		}

		ass::Subtitles subtitlesASS;
		subtitlesASS.dialogues = dialogues;
		subtitlesASS.styles = styles;

		return subtitlesASS;
	}
}