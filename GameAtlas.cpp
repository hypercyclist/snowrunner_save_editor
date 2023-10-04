#include "GameAtlas.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include "Utils.h"
#include "Region.h"
#include "Map.h"
#include "Task.h"
#include "Upgrade.h"
#include "Localization.h"
#include <string>
#include <algorithm>

GameAtlas::GameAtlas() :
    m_regions(),
    m_localization(nullptr)
{ }

void GameAtlas::setLocalization(Localization* _localization)
{
    m_localization = _localization;
}

void GameAtlas::addRegion(Region* _region)
{
    m_regions[_region->code()] = _region;
}

std::map<std::string, Region*> GameAtlas::regions()
{
    return m_regions;
}

Region* GameAtlas::region(std::string _regionCode)
{
    return m_regions.find(_regionCode) != m_regions.end() ?
        m_regions[_regionCode] : nullptr;
}

// Нет проверок на валидность JSON.
void GameAtlas::createGameAtlasData(std::string _filename)
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

        Region* region = m_regions[regionCode];
        if (!region)
        {
            region = new Region(regionCode);

            for (const auto& languagePair : m_localization->languageTextNames())
            {
                std::string regionTranslate = m_localization->getLocalization(regionCode, languagePair.first);
                regionTranslate = Utils::cutLongCountryName(regionTranslate);

                region->setName(languagePair.first, regionTranslate);
            }

            addRegion(region);
        }
        Map* map = region->map(mapCode);
        if (!map)
        {
            map = new Map(mapCode);

            for (const auto& languagePair : m_localization->languageTextNames())
            {
                // Тут нужно загрузить для всех языков.
                std::string mapTranslate = m_localization->getLocalization(mapCode, languagePair.first);
                if (mapTranslate == "none")
                    mapTranslate = m_localization->getLocalization(mapCode + "_NAME", languagePair.first);
                if (mapTranslate == "none")
                    mapTranslate = m_localization->getLocalization(mapCode + "_NEW_NAME", languagePair.first);
                if (mapTranslate == "none")
                    mapTranslate = m_localization->getLocalization("LEVEL_" + mapCode + "_NAME", languagePair.first);
                if (mapTranslate == "none")
                    mapTranslate = m_localization->getLocalization("LEVEL_" + mapCode, languagePair.first);

                mapTranslate = Utils::cutSlash(mapTranslate);

                map->setName(languagePair.first, mapTranslate);
            }

            region->addMap(map);
        }

        Task* task = new Task(taskCode);

        for (const auto& languagePair : m_localization->languageTextNames())
        {
            std::string taskTranslate = m_localization->getLocalization(taskCode, languagePair.first);
            task->setName(languagePair.first, taskTranslate);
        }

        map->addTask(task);
    }
}

void GameAtlas::createUpgradesData(std::string _saveFileName,
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
        jsonDocument["CompleteSave"]["SslValue"]["upgradesGiverData"];

    for (auto levelIt = ppdObject.MemberBegin(); levelIt != ppdObject.MemberEnd(); levelIt++)
    {
        for (auto upgradesIt = levelIt->value.MemberBegin(); upgradesIt != levelIt->value.MemberEnd(); upgradesIt++)
        {
            std::string upgradeCode = upgradesIt->name.GetString();

            Region* region = m_regions[upgradeCode.substr(0, 5)];
            if (!region) {
                continue;
            }

            Map* map = region->map(upgradeCode.substr(0, 8));
            if (!map) {
                continue;
            }

            Upgrade* upgrade = new Upgrade(upgradeCode);

            auto indexOfUpgrade = cacheFile.find(upgradeCode);
            if (indexOfUpgrade != std::string::npos)
            {
                auto indexOfUpgradeMiddleCodeStart = cacheFile.find("[", indexOfUpgrade);
                auto indexOfUpgradeMiddleCodeEnd = cacheFile.find("]", indexOfUpgradeMiddleCodeStart);
                std::string mCode = cacheFile.substr(
                    indexOfUpgradeMiddleCodeStart, indexOfUpgradeMiddleCodeEnd - indexOfUpgradeMiddleCodeStart);
                mCode.erase(std::remove(mCode.begin(), mCode.end(), ' '), mCode.end());
                mCode.erase(std::remove(mCode.begin(), mCode.end(), '\r'), mCode.end());
                mCode.erase(std::remove(mCode.begin(), mCode.end(), '\n'), mCode.end());
                mCode.erase(std::remove(mCode.begin(), mCode.end(), '['), mCode.end());
                mCode.erase(std::remove(mCode.begin(), mCode.end(), ']'), mCode.end());
                mCode.erase(std::remove(mCode.begin(), mCode.end(), '\"'), mCode.end());
                upgrade->setMiddleCode(mCode);
                m_localization->addLocalizationsTemplate(mCode, "none");
                for (const auto& languagePair : m_localization->languageTextNames())
                {
                    std::string translation = m_localization->getLocalization(mCode, languagePair.first);
                    translation = translation == "none" ? mCode : translation;
                    upgrade->setName(languagePair.first, translation);
                }
            }

            map->addUpgrade(upgrade);
        }
    }

    for(auto regionIt = m_regions.begin(); regionIt != m_regions.end();)
    {
        if (!regionIt->second)
        {
            m_regions.erase(regionIt++);
        }
        else
        {
            regionIt++;
        }
    }
}

// Считаем, что если файл открылся и там JSON, то все хорошо.
bool GameAtlas::loadGameAtlas(std::string _filename)
{
    std::ifstream readStream(_filename);

    if (!readStream.is_open()) {
        return false;
    }

    std::stringstream buffer;
    buffer << readStream.rdbuf();
    readStream.close();
    std::string tasksText = buffer.str();

    rapidjson::Document tasksJsonDocument;
    tasksJsonDocument.Parse(tasksText.c_str());

    if (!tasksJsonDocument.IsObject()) {
        return false;
    }

    rapidjson::Value& gameAtlasObject = tasksJsonDocument["GameAtlas"];
    for (auto regionIt = gameAtlasObject.MemberBegin(); regionIt != gameAtlasObject.MemberEnd(); regionIt++)
    {
        rapidjson::Value& regionObject = regionIt->value;

        Region* region = new Region(regionIt->name.GetString());

        for (auto mapIt = regionObject.MemberBegin(); mapIt != regionObject.MemberEnd(); mapIt++)
        {
            rapidjson::Value& mapObject = mapIt->value;

            Map* map = new Map(mapIt->name.GetString());

            rapidjson::Value& tasksObject = mapObject["Tasks"];
            for (auto taskIt = tasksObject.MemberBegin(); taskIt != tasksObject.MemberEnd(); taskIt++)
            {
                Task* task = new Task(taskIt->name.GetString());
                for (const auto& languagePair : m_localization->languageTextNames())
                {
                    task->setName(languagePair.first, m_localization->getLocalization(task->code(), languagePair.first));
                }
                map->addTask(task);
            }

            rapidjson::Value& upgradesObject = mapObject["Upgrades"];
            for (auto upgradeIt = upgradesObject.MemberBegin(); upgradeIt != upgradesObject.MemberEnd(); upgradeIt++)
            {
                Upgrade* upgrade = new Upgrade(upgradeIt->name.GetString());
                upgrade->setMiddleCode(upgradeIt->value["middleCode"].GetString());
                for (const auto& languagePair : m_localization->languageTextNames())
                {
                    upgrade->setName(languagePair.first, m_localization->getLocalization(upgrade->code(), languagePair.first));
                }
                map->addUpgrade(upgrade);
            }

            for (const auto& languagePair : m_localization->languageTextNames())
            {
                std::string mapTranslate = m_localization->getLocalization(map->code(), languagePair.first);
                if (mapTranslate == "none")
                    mapTranslate = m_localization->getLocalization(map->code() + "_NAME", languagePair.first);
                if (mapTranslate == "none")
                    mapTranslate = m_localization->getLocalization(map->code() + "_NEW_NAME", languagePair.first);
                if (mapTranslate == "none")
                    mapTranslate = m_localization->getLocalization("LEVEL_" + map->code() + "_NAME", languagePair.first);
                if (mapTranslate == "none")
                    mapTranslate = m_localization->getLocalization("LEVEL_" + map->code(), languagePair.first);
                map->setName(languagePair.first, mapTranslate);
            }

            region->addMap(map);
        }

        for (const auto& languagePair : m_localization->languageTextNames())
        {
            region->setName(languagePair.first, m_localization->getLocalization(region->code(), languagePair.first));
        }

        addRegion(region);
    }

    return true;
}

// Если файла нет - создать. Если в нем есть инфа мержить ручной перевод.
void GameAtlas::saveGameAtlasData(std::string _filename)
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

    for (const auto& regionPair : m_regions)
    {
        Region* region = regionPair.second;

        if (!region) {
            continue;
        }

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

            rapidjson::Value tasksBlockObject;
            tasksBlockObject.SetObject();

            std::string tasksBlockNameString = "Tasks";
            rapidjson::Value tasksBlockName;
            tasksBlockName.SetString(tasksBlockNameString.c_str(), tasksBlockNameString.length(), allocator);

            for (const auto& taskPair : map->tasks())
            {
                Task* task = taskPair.second;

                rapidjson::Value taskObject;
                taskObject.SetObject();

                rapidjson::Value taskCode;
                taskCode.SetString(task->code().c_str(), task->code().length(), allocator);

                tasksBlockObject.AddMember(taskCode, taskObject, allocator);
            }
            mapObject.AddMember(tasksBlockName, tasksBlockObject, allocator);

            rapidjson::Value upgradesBlockObject;
            upgradesBlockObject.SetObject();

            std::string upgradesBlockNameString = "Upgrades";
            rapidjson::Value upgradesBlockName;
            upgradesBlockName.SetString(upgradesBlockNameString.c_str(), upgradesBlockNameString.length(), allocator);

            for (const auto& upgradePair : map->upgrades())
            {
                Upgrade* upgrade = upgradePair.second;

                rapidjson::Value upgradeObject;
                upgradeObject.SetObject();

                rapidjson::Value upgradeName;
                upgradeName.SetString(upgrade->code().c_str(), upgrade->code().length(), allocator);

                rapidjson::Value upgradeMiddleCode;
                upgradeMiddleCode.SetString(upgrade->middleCode().c_str(), upgrade->middleCode().length(), allocator);
                upgradeObject.AddMember("middleCode", upgradeMiddleCode, allocator);

                upgradesBlockObject.AddMember(upgradeName, upgradeObject, allocator);
            }
            mapObject.AddMember(upgradesBlockName, upgradesBlockObject, allocator);

            regionObject.AddMember(mapCode, mapObject, allocator);
        }
        gameAtlas.AddMember(regionCode, regionObject, allocator);
    }

    if (databaseJsonDocument.HasMember("GameAtlas"))
    {
        databaseJsonDocument["GameAtlas"] = gameAtlas;
    }
    else
    {
        databaseJsonDocument.AddMember("GameAtlas", gameAtlas, allocator);
    }

    rapidjson::StringBuffer writeBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(writeBuffer);
    databaseJsonDocument.Accept(writer);

    std::ofstream writeStream(_filename);
    writeStream << writeBuffer.GetString();
    writeStream.close();
}

std::vector<Task*> GameAtlas::completedTasks()
{
    std::vector<Task*> result;
    for (const auto& regionPair : m_regions)
    {
        Region* region = regionPair.second;

        if (!region) {
            continue;
        }

        for (const auto& mapPair : region->maps())
        {
            Map* map = mapPair.second;

            for (const auto& taskPair : map->tasks())
            {
                Task* task = taskPair.second;
                if (task->complete())
                {
                    result.push_back(task);
                }
            }
        }
    }
    return result;
}

void GameAtlas::setTasksCompleteFromVectorCodes(std::vector<std::string> _codes)
{
    for (const auto& regionPair : m_regions)
    {
        Region* region = regionPair.second;

        if (!region) {
            continue;
        }

        for (const auto& mapPair : region->maps())
        {
            Map* map = mapPair.second;

            for (const auto& taskPair : map->tasks())
            {
                Task* task = taskPair.second;
                if (std::find(_codes.begin(), _codes.end(), task->code())
                    != _codes.end() )
                {
                    task->setComplete(true);
                }
                else
                {
                    task->setComplete(false);
                }
            }
        }
    }
}

std::vector<Upgrade*> GameAtlas::receivedUpgrades()
{
    std::vector<Upgrade*> result;
    for (const auto& regionPair : m_regions)
    {
        Region* region = regionPair.second;

        if (!region) {
            continue;
        }

        for (const auto& mapPair : region->maps())
        {
            Map* map = mapPair.second;

            for (const auto& upgradePair : map->upgrades())
            {
                Upgrade* upgrade = upgradePair.second;
                if (upgrade->received())
                {
                    result.push_back(upgrade);
                }
            }
        }
    }
    return result;
}

void GameAtlas::setUpgradesReceivedFromVectorCodes(std::vector<std::string> _codes)
{
    for (const auto& regionPair : m_regions)
    {
        Region* region = regionPair.second;

        if (!region) {
            continue;
        }

        for (const auto& mapPair : region->maps())
        {
            Map* map = mapPair.second;

            for (const auto& upgradePair : map->upgrades())
            {
                Upgrade* upgrade = upgradePair.second;
                if (std::find(_codes.begin(), _codes.end(), upgrade->code())
                    != _codes.end() )
                {
                    upgrade->setReceived(true);
                }
                else
                {
                    upgrade->setReceived(false);
                }
            }
        }
    }
}
