#pragma once
#include "jmdict.h"
#include <nlohmann/json.hpp>

inline void to_json(nlohmann::json& jsonObject, const KanjiElement& kanjiElement)
{
    using namespace nlohmann;

    jsonObject = json {
        { "word", kanjiElement.word },
        { "info", kanjiElement.info },
        { "priorities", kanjiElement.priorities }
    };
}

inline void to_json(nlohmann::json& jsonObject, const ReadingElement& readingElement)
{
    using namespace nlohmann;

    jsonObject = json{
        { "reading", readingElement.reading },
        { "is_no_kanji", readingElement.isNoKanji },
        { "restrictions", readingElement.restrictions },
        { "info", readingElement.info },
        { "priorities", readingElement.priorities }
    };
}

inline void to_json(nlohmann::json& jsonObject, const Gloss& gloss)
{
    using namespace nlohmann;

    jsonObject = json{
        { "glossary", gloss.glossary },
        { "language", gloss.language },
        { "gender", gloss.gender },
        { "type", gloss.type },
    };
}

inline void to_json(nlohmann::json& jsonObject, const LanguageSource& languageSource)
{
    using namespace nlohmann;

    jsonObject = json{
        { "source_language", languageSource.sourceLanguage },
        { "ls_type", languageSource.lsType },
        { "is_wasei", languageSource.isWasei },
        { "source_language_meaning", languageSource.sourceLanguageMeaning },
    };
}

inline void to_json(nlohmann::json& jsonObject, const SenseElement& senseElement)
{
    using namespace nlohmann;

    jsonObject = json{
        { "applies_to_kanji", senseElement.appliesToKanji },
        { "applies_to_lexeme", senseElement.appliesToLexeme },
        { "cross_reference", senseElement.crossReference },
        { "antonym", senseElement.antonym },
        { "part_of_speech", senseElement.partOfSpeech },
        { "field_of_study", senseElement.fieldOfStudy },
        { "misc", senseElement.misc },
        { "language_source", senseElement.languageSource },
        { "dialect", senseElement.dialect },
        { "glosses", senseElement.glosses },
        { "sense_information", senseElement.senseInformation },
    };
}

inline void to_json(nlohmann::json& jsonObject, const JMDictEntry& jmDictEntry)
{
    using namespace nlohmann;

    jsonObject = json{
        { "id", jmDictEntry.id },
        { "kanji_elements", jmDictEntry.kanjiElements },
        { "reading_elements", jmDictEntry.readingElements },
        { "sense_elements", jmDictEntry.senseElements },
    };
}