#include "Database.h"
#include <QDir>
#include <fstream>
#include <sstream>
#include <iostream>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include "Localization.h"
#include "Utils.h"
#include "GameAtlas.h"
#include "Region.h"
#include "Map.h"
#include "Task.h"
#include "Upgrade.h"

Database::Database() :
    m_localization(nullptr),
    m_gameAtlas(nullptr)
//    m_tasks()
{
    m_localization = new Localization();
    m_localization->loadLocalizations();
    m_localization->setDefaultLanguage(Language::RUSSIAN);

    m_gameAtlas = new GameAtlas();
}

void Database::createDatabase()
{
    std::string currentPath = QDir::currentPath().toStdString();
    std::string databasePath = currentPath + "/database/database.json";

    // Parse file. Search regions, maps, tasks. Add transcription.
    loadRegionsMapsTasksData(
        currentPath + "/database/generator_materials/tasks.json");

    loadUpgradesData(
        currentPath + "/database/generator_materials/CompleteSave.cfg",
        currentPath + "/database/generator_materials/initial.cache_block");

    saveRegionsMapsTasksData(databasePath);
    saveLocalizationCache(databasePath);

    // Parse save for upgrades list.
    // Parse initial.cache_block for upgrades middle code.
    // Find translation to middle code of upgrades.

    // Money, Expirience, Rank... POPUP_TRAILER_MONEY, CODEX_LEVEL_UP_HEADER, UI_NGP_RANK_10
//    UI_MINIMAP_TAB_HEADER_CONTRACTS,
//    UI_MINIMAP_TAB_HEADER_TASKS
}

void Database::loadDatabase()
{

}

// Нет проверок на валидность JSON.
void Database::loadRegionsMapsTasksData(std::string _filename)
{
    std::ifstream readStream(_filename);
    std::stringstream buffer;
    buffer << readStream.rdbuf();
    readStream.close();
    std::string tasksText = buffer.str();

    rapidjson::Document tasksJsonDocument;
    tasksJsonDocument.Parse(tasksText.c_str());
    rapidjson::Value& tasksJsonArray = tasksJsonDocument["tasks"].GetArray();

    for (rapidjson::SizeType i = 0; i < tasksJsonArray.Size(); i++)
    {
        std::string taskCode = tasksJsonArray[i]["code"].GetString();
        std::string regionCode = taskCode.substr(0, 5);
        std::string mapCode = taskCode.substr(0, 8);

        Region* region = m_gameAtlas->region(regionCode);
        if (!region)
        {
            region = new Region(regionCode);

            // Тут нужно загрузить для всех языков.
            std::string regionTranslate = m_localization->getLocalization(regionCode);
            regionTranslate = Utils::cutLongCountryName(regionTranslate);

            region->setName(Language::RUSSIAN, regionTranslate);

            m_gameAtlas->addRegion(region);
        }
        Map* map = region->map(mapCode);
        if (!map)
        {
            map = new Map(mapCode);

            // Тут нужно загрузить для всех языков.
            std::string mapTranslate = m_localization->getLocalization(mapCode);
            if (mapTranslate == "none")
                mapTranslate = m_localization->getLocalization(mapCode + "_NAME");
            if (mapTranslate == "none")
                mapTranslate = m_localization->getLocalization(mapCode + "_NEW_NAME");
            if (mapTranslate == "none")
                mapTranslate = m_localization->getLocalization("LEVEL_" + mapCode + "_NAME");
            if (mapTranslate == "none")
                mapTranslate = m_localization->getLocalization("LEVEL_" + mapCode);

            mapTranslate = Utils::cutSlash(mapTranslate);

            map->setName(Language::RUSSIAN, mapTranslate);

            region->addMap(map);
        }

        Task* task = new Task(taskCode);

        // Тут нужно загрузить для всех языков.
        std::string taskTranslate = m_localization->getLocalization(taskCode);
        task->setName(Language::RUSSIAN, taskTranslate);

        map->addTask(task);
    }
}

// Если файла нет - создать. Если в нем есть инфа мержить ручной перевод.
void Database::saveRegionsMapsTasksData(std::string _filename)
{
    std::ifstream readStream(_filename);
    std::stringstream readBuffer;
    readBuffer << readStream.rdbuf();
    readStream.close();
    std::string databaseText = readBuffer.str();

    rapidjson::Document databaseJsonDocument;
    databaseJsonDocument.Parse(databaseText.c_str());
    auto allocator = databaseJsonDocument.GetAllocator();

    if (databaseText.size() == 0)
    {
        databaseJsonDocument.SetObject();
    }

    rapidjson::Value gameAtlas;
    gameAtlas.SetObject();
    for (const auto& regionPair : m_gameAtlas->regions())
    {
        Region* region = regionPair.second;

        rapidjson::Value regionObject;
        regionObject.SetObject();

        rapidjson::Value regionCode;
        regionCode.SetString(region->code().c_str(), region->code().length(), allocator);

        for (const auto& mapPair : region->maps())
        {
            Map* map = mapPair.second;

            rapidjson::Value mapObject;
            mapObject.SetObject();

            rapidjson::Value mapCode;
            mapCode.SetString(map->code().c_str(), map->code().length(), allocator);

            for (const auto& taskPair : map->tasks())
            {
                Task* task = taskPair.second;

                rapidjson::Value taskObject;
                taskObject.SetObject();

                rapidjson::Value taskCode;
                taskCode.SetString(task->code().c_str(), task->code().length(), allocator);

                mapObject.AddMember(taskCode, taskObject, allocator);
            }
            regionObject.AddMember(mapCode, mapObject, allocator);
        }
        gameAtlas.AddMember(regionCode, regionObject, allocator);
    }
    databaseJsonDocument.AddMember("GameAtlas", gameAtlas, allocator);

    rapidjson::StringBuffer writeBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(writeBuffer);
    databaseJsonDocument.Accept(writer);

    std::ofstream writeStream(_filename);
    writeStream << writeBuffer.GetString();
    writeStream.close();
}

void Database::saveLocalizationCache(std::string _filename)
{
    std::ifstream readStream(_filename);
    std::stringstream readBuffer;
    readBuffer << readStream.rdbuf();
    readStream.close();
    std::string databaseText = readBuffer.str();

    rapidjson::Document databaseJsonDocument;
    databaseJsonDocument.Parse(databaseText.c_str());
    auto allocator = databaseJsonDocument.GetAllocator();

    rapidjson::Value localization;
    localization.SetObject();

    for (const auto& languagePair : m_localization->getLocalizationCache())
    {
        rapidjson::Value languageObject;
        languageObject.SetObject();

        std::string languageCode = m_localization->languageTextName(languagePair.first);
        rapidjson::Value languageCodeValue;
        languageCodeValue.SetString(languageCode.c_str(), languageCode.length(), allocator);

        for (const auto& wordPair : languagePair.second)
        {
            rapidjson::Value wordString;
            wordString.SetString(wordPair.second.c_str(), wordPair.second.length(), allocator);

            rapidjson::Value wordCode;
            wordCode.SetString(wordPair.first.c_str(), wordPair.first.length(), allocator);

            languageObject.AddMember(wordCode, wordString, allocator);

        }
        localization.AddMember(languageCodeValue, languageObject, allocator);
    }
    databaseJsonDocument.AddMember("Localization", localization, allocator);

    rapidjson::StringBuffer writeBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(writeBuffer);
    databaseJsonDocument.Accept(writer);

    std::ofstream writeStream(_filename);
    writeStream << writeBuffer.GetString();
    writeStream.close();
}

void Database::loadUpgradesData(std::string _saveFileName,
    std::string _initialCacheBlockFileName)
{
    // Read initial.cache_block.
    std::ifstream readStream(_initialCacheBlockFileName, std::ios::binary);
    std::stringstream buffer;
    buffer << readStream.rdbuf();
    readStream.close();
    std::string cacheFile = buffer.str();

    // Read upgrades.json.
    readStream.open(_saveFileName);
    buffer.str(std::string());
    buffer << readStream.rdbuf();
    readStream.close();
    std::string saveFile = buffer.str();

    rapidjson::Document jsonDocument;
    jsonDocument.Parse(saveFile.c_str());

    rapidjson::Value& ppdObject =
        jsonDocument["CompleteSave"]["SslValue"]["upgradesGiverData"]; // GetObject().

    for (rapidjson::SizeType i = 0; i < ppdObject.MemberCount(); i++)
    {
        for (auto upgradesIt = ppdObject[i].MemberBegin(); upgradesIt != ppdObject[i].MemberEnd(); upgradesIt++)
        {
            std::string upgradeCode = upgradesIt->name.GetString();
            Map* map = m_gameAtlas->region(upgradeCode.substr(0, 5))->map(upgradeCode.substr(0, 8));

            Upgrade* upgrade = new Upgrade(upgradeCode);

            auto indexOfUpgrade = cacheFile.find(upgradeCode);
            if (indexOfUpgrade != std::string::npos)
            {
                auto indexOfUpgradeMiddleCodeStart = cacheFile.find("[", indexOfUpgrade);
                auto indexOfUpgradeMiddleCodeEnd = cacheFile.find("]", indexOfUpgradeMiddleCodeStart);
                std::string code = cacheFile.substr(
                    indexOfUpgradeMiddleCodeStart, indexOfUpgradeMiddleCodeEnd - indexOfUpgradeMiddleCodeStart);
                code.erase(std::remove(code.begin(), code.end(), ' '), code.end());
                code.erase(std::remove(code.begin(), code.end(), '\r'), code.end());
                code.erase(std::remove(code.begin(), code.end(), '\n'), code.end());
                code.erase(std::remove(code.begin(), code.end(), '['), code.end());
                code.erase(std::remove(code.begin(), code.end(), ']'), code.end());
                code.erase(std::remove(code.begin(), code.end(), '\"'), code.end());
                // std::cout << m_localization->getLocalization(
                //     QString::fromStdString(code)).toStdString() << std::endl;
                std::cout << upgradeCode << " : " << code << std::endl;
            }
            map->addUpgrade(upgrade);
        }
    }
}
