#include "pch.h"
#include "common.h"
#include "jmdict/jmdict_json.h"
#include "jmdict/jmdict_parser.h"

namespace fs = std::filesystem;
using namespace nlohmann;

static constexpr const char* JsonExtension = ".json";
static std::string OutputFolder;

extern "C" CROW_API void setOutputFolder(const char* folderPath)
{
    OutputFolder = folderPath;
}

extern "C" CROW_API bool parse(const char* filepath)
{
    // Load XML:
    pugi::xml_document document;
    if (!document.load_file(filepath))
    {
        std::string error = "Failed to load file: " + std::string(filepath);
        return false;
    }

    // Parse Dictionary:
    json dictionaryData;
    for (const auto& entry : document.first_child().children())
    {
        JMDictEntry jmEntry;

        for (const auto& child : entry)
        {
            auto name = child.name();
            auto value = child.text().as_string();

            if (strcmp(name, "ent_seq") == 0)
                jmEntry.id = parseEntitySequence(child);

            else if (strcmp(name, "k_ele") == 0)
            {
                KanjiElement kanjiElement = parseKanjiElements(child);
                jmEntry.kanjiElements.push_back(kanjiElement);
            }

            else if (strcmp(name, "r_ele") == 0)
            {
                ReadingElement readingElement = parseReadingElement(child);
                jmEntry.readingElements.push_back(readingElement);
            }

            else if (strcmp(name, "sense") == 0)
            {
                SenseElement senseElement = parseSenseElement(child);
                jmEntry.senseElements.push_back(senseElement);
            }
        }

        dictionaryData.push_back(jmEntry);
    }
    
    // Save resulting JSON:
    fs::path path(filepath);
    std::string outputFilename= path.stem().string() + JsonExtension;

    fs::path outputFolderPath = !OutputFolder.empty() ? OutputFolder : path.parent_path();

    std::ofstream output(outputFolderPath / outputFilename);
    output << std::setw(2) << dictionaryData;

    return true;
}