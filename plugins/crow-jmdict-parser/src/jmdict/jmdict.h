#pragma once
#include <string>
#include <vector>

struct KanjiElement                             // k_ele
{
    std::string word;                           // keb
    std::string info;                           // k_inf
    std::vector<std::string> priorities;        // k_pri
};

struct ReadingElement                           // r_ele
{
    std::string reading;                        // reb
    bool isNoKanji = false;                     // re_nokanji
    std::vector<std::string> restrictions;      // re_restr
    std::string info;                           // re_inf
    std::vector<std::string> priorities;        // re_pri
};

struct Gloss
{
    std::string glossary;           // gloss
    std::string language;           // gloss xml:lang
    std::string gender;             // g_gend
    std::string type;               // g_type
};

struct LanguageSource
{
    std::string sourceLanguage;                // xml:lang
    std::string lsType;                        // ls_type
    bool isWasei;                              // wasei
    std::string sourceLanguageMeaning;
};

struct SenseElement
{
    std::string appliesToKanji;                   // stagk
    std::string appliesToLexeme;                  // stagr
    std::string crossReference;                   // xref
    std::string antonym;                          // ant
    std::vector<std::string> partOfSpeech;        // pos
    std::string fieldOfStudy;                     // field
    std::vector<std::string> misc;                // misc
    LanguageSource languageSource;                // lsource
    std::string dialect;                          // dial
    std::vector<Gloss> glosses;                   // gloss
    std::string senseInformation;                 // s_inf
};

struct JMDictEntry                                     // entry
{
    unsigned int id = 0;                               // ent_seq
    std::vector<KanjiElement> kanjiElements;           // k_ele
    std::vector<ReadingElement> readingElements;       // r_ele
    std::vector<SenseElement> senseElements;           // sense
};