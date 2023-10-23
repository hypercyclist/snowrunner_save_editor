#include "Database.h"
#include <QDir>
#include <iostream>
#include "Localization.h"
#include "GameAtlas.h"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
//#include "NewGamePlusSettings.h"
#include <fstream>
#include <sstream>
#include "Region.h"
#include "Map.h"
#include "Task.h"
#include "Utils.h"

Database::Database() :
    m_localization(nullptr),
    m_gameAtlas(nullptr),
    m_newGamePlusSettings(nullptr)
{
    m_localization = new Localization();
    m_gameAtlas = new GameAtlas();
    m_gameAtlas->setLocalization(m_localization);
//    m_newGamePlusSettings = new NewGamePlusSettings();
}

void Database::createDatabase(std::string _databasePath)
{
    std::string tasksPath = _databasePath + "/generator_materials/tasks.json";
    std::string initialCacheBlockPath = _databasePath + "/generator_materials/initial.cache_block";
    std::string savePath = _databasePath + "/generator_materials/CompleteSave.cfg";

    m_localization->createLocalizations();

    // Parse file. Search regions, maps, tasks. Add transcription.
    m_gameAtlas->createGameAtlasData(tasksPath);

    // Parse save for upgrades list.
    // Parse initial.cache_block for upgrades middle code.
    // Find translation to middle code of upgrades.

    // Перенести эту функцию в createGameAtlasData?
    m_gameAtlas->createUpgradesData(savePath, initialCacheBlockPath);
    m_gameAtlas->createTrucksData();
    m_gameAtlas->connectUpgradesWithTrucks();


    m_gameAtlas->saveGameAtlasData(_databasePath);
    m_localization->saveLocalizationCache(_databasePath);
}

void Database::createDatabase2(std::string _databasePath)
{
    GameAtlas* gameAtlas = new GameAtlas();

    std::string tasksPath = _databasePath + "/generator_materials/tasks.json";
    std::string initialCacheBlockPath = _databasePath + "/generator_materials/initial.cache_block";
    std::string savePath = _databasePath + "/generator_materials/CompleteSave.cfg";

    m_localization->createLocalizations();

    std::ifstream readStream(tasksPath);
    std::stringstream buffer;
    buffer << readStream.rdbuf();
    readStream.close();
    std::string tasksText = buffer.str();

    rapidjson::Document tasksJsonDocument;
    tasksJsonDocument.Parse(tasksText.c_str());
    rapidjson::Value& tasksJsonArray = tasksJsonDocument["tasks"].GetArray();

    for (rapidjson::Value& taskJMO : tasksJsonArray.GetArray())
    {
        std::string taskCode = taskJMO["code"].GetString();
        std::string regionCode = taskCode.substr(0, 5);
        std::string mapCode = taskCode.substr(0, 8);

        Region* region = gameAtlas->region(regionCode);
        if (!region)
        {
            region = new Region(regionCode);
            std::string regionTranslationCode = regionCode;
            for (const auto& languagePair : m_localization->languageTextNames())
            {
                std::string translation = m_localization->localization(regionTranslationCode, languagePair.first);
                translation = Utils::cutLongCountryName(translation);
                region->setName(languagePair.first, translation);
            }
            region->setTranslationCode(regionTranslationCode);
            gameAtlas->addRegion(region);
        }
        Map* map = region->map(mapCode);
        if (!map)
        {
            map = new Map(mapCode);
            std::string mapTranslationCode = mapCode;
            for (const auto& languagePair : m_localization->languageTextNames())
            {
                std::string translation = m_localization->localization(mapTranslationCode, languagePair.first);
                if (translation == "none") {
                    mapTranslationCode = mapCode + "_NAME";
                    translation = m_localization->localization(mapTranslationCode, languagePair.first);
                }
                if (translation == "none") {
                    mapTranslationCode = mapCode + "_NEW_NAME";
                    translation = m_localization->localization(mapTranslationCode, languagePair.first);
                }
                if (translation == "none") {
                    mapTranslationCode = "LEVEL_" + mapCode + "_NAME";
                    translation = m_localization->localization(mapTranslationCode, languagePair.first);
                }
                if (translation == "none") {
                    mapTranslationCode = "LEVEL_" + mapCode;
                    translation = m_localization->localization(mapTranslationCode, languagePair.first);
                }
                translation = Utils::cutSlash(translation);
                map->setName(languagePair.first, translation);
            }
            map->setTranslationCode(mapTranslationCode);
            region->addMap(map);
        }

        Task* task = new Task(taskCode);

        for (const auto& languagePair : m_localization->languageTextNames())
        {
            std::string taskTranslate = m_localization->localization(taskCode, languagePair.first);
            task->setName(languagePair.first, taskTranslate);
        }

        map->addTask(task);
    }

}

bool Database::loadDatabase(std::string _databasePath)
{
    bool localizationLoaded = m_localization->loadLocalizations(_databasePath + "/database.json");
    if (!localizationLoaded) {
        return false;
    }
    m_localization->setDefaultLanguage(Language::RUSSIAN);

    bool gameAtlasLoaded = m_gameAtlas->loadGameAtlas(_databasePath + "/database.json");
    if (!gameAtlasLoaded) {
        return false;
    }

    return true;
}

Localization* Database::localization()
{
    return m_localization;
}

GameAtlas* Database::gameAtlas()
{
    return m_gameAtlas;
}

NewGamePlusSettings* Database::newGamePlusSettings()
{
    return m_newGamePlusSettings;
}
