#pragma once
#include "jmdict.h"
#include <pugixml.hpp>

inline int parseEntitySequence(const pugi::xml_node& entry)
{
    return entry.text().as_int();
}

inline KanjiElement parseKanjiElements(const pugi::xml_node& entry)
{
    KanjiElement kanjiElement;

    for (const auto& child : entry.children())
    {
        auto name = child.name();
        auto value = child.text().as_string();

        if (strcmp(name, "keb") == 0)
            kanjiElement.word = value;

        else if (strcmp(name, "ke_inf") == 0)
            kanjiElement.info = value;

        else if (strcmp(name, "ke_pri") == 0)
            kanjiElement.priorities.push_back(value);
    }

    return kanjiElement;
}

inline ReadingElement parseReadingElement(const pugi::xml_node& entry)
{
    ReadingElement readingElement;

    for (const auto& child : entry.children())
    {
        auto name = child.name();
        auto value = child.text().as_string();

        if (strcmp(name, "reb") == 0)
            readingElement.reading = value;

        if (strcmp(name, "re_nokanji") == 0)
            readingElement.isNoKanji = true;

        if (strcmp(name, "re_inf") == 0)
            readingElement.info = value;

        if (strcmp(name, "re_pri") == 0)
            readingElement.priorities.push_back(value);
    }

    return readingElement;
}

static inline Gloss parseLanguageGlossary(const pugi::xml_node& entry)
{
    Gloss glossary;
    glossary.glossary = entry.text().as_string();

    for (const auto& attribute : entry.attributes())
    {
        if (strcmp(attribute.name(), "lang") == 0)
            glossary.language = attribute.as_string();

        if (strcmp(attribute.name(), "g_gend") == 0)
            glossary.gender = attribute.as_string();

        if (strcmp(attribute.name(), "gtype") == 0)
            glossary.type = attribute.as_string();
    }

    return glossary;
}

static inline LanguageSource parseLanguageSource(const pugi::xml_node& entry)
{
    LanguageSource languageSource;

    languageSource.sourceLanguageMeaning = entry.text().as_string();

    for (const auto& attribute : entry.attributes())
    {
        if (strcmp(attribute.name(), "lang") == 0)
            languageSource.sourceLanguage = attribute.as_string();

        if (strcmp(attribute.name(), "ls_wasei") == 0)
        {
            if(attribute.as_bool())
                languageSource.isWasei = true;
        }
    }

    return languageSource;
}

inline SenseElement parseSenseElement(const pugi::xml_node& entry)
{
    SenseElement senseElement;

    for (const auto& child : entry.children())
    {
        auto name = child.name();
        auto value = child.text().as_string();

        if (strcmp(name, "stagk") == 0)
            senseElement.appliesToKanji = value;
        else if (strcmp(name, "stagr") == 0)
            senseElement.appliesToLexeme = value;
        else if (strcmp(name, "xref") == 0)
            senseElement.crossReference = value;
        else if (strcmp(name, "ant") == 0)
            senseElement.antonym = value;
        else if (strcmp(name, "pos") == 0)
            senseElement.partOfSpeech.push_back(value);
        else if (strcmp(name, "field") == 0)
            senseElement.fieldOfStudy = value;
        else if (strcmp(name, "misc") == 0)
            senseElement.misc.push_back(value);
        else if (strcmp(name, "lsource") == 0)
            senseElement.languageSource = parseLanguageSource(entry);
        else if (strcmp(name, "dial") == 0)
            senseElement.dialect = value;
        else if (strcmp(name, "gloss") == 0)
            senseElement.glosses.push_back(parseLanguageGlossary(entry));
        else if (strcmp(name, "s_inf") == 0)
            senseElement.senseInformation = value;
    }
    return senseElement;
}