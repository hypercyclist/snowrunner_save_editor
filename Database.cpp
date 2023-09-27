#include "Database.h"
#include <QFile>
#include <QSet>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "Localization.h"
#include <QDir>

#include <QDir>
#include <fstream>
#include <sstream>
#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>

#include "Localization.h"
#include <QString>
#include "Utils.h"

Database::Database() :
    m_localization(nullptr),
    m_tasks()
{
    m_localization = new Localization();
    m_localization->loadLocalizations();
    m_localization->setDefaultLanguage(Language::RUSSIAN);
}

void Database::createDatabase()
{
    // Parse file. Search regions, maps, tasks. Add transcription.
    loadRegionsMapsTasksData(QDir::currentPath().toStdString() +
        "/database/generator_materials/tasks.json");

    saveRegionsMapsTasksData(QDir::currentPath().toStdString() +
        "/database/database.json");



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

        auto regionIt = m_tasks.find(regionCode);
        if (regionIt != m_tasks.end())
        {
            m_tasks.insert(std::pair<std::string, std::map<std::string,
                std::map<std::string, std::map<std::string, std::string>>>>());

            std::map<std::string, std::string> regionInfo;

            // Тут нужно загрузить для всех языков.
            std::string regionRussianTranslate = m_localization->getLocalization(regionCode);
            regionRussianTranslate = Utils::cutLongCountryName(regionRussianTranslate);
            regionInfo.insert(std::pair<std::string, std::string>("rus", regionRussianTranslate));

            m_regions.insert(std::pair<std::string,
                std::map<std::string, std::string>>(regionCode, regionInfo));
        }
        auto mapIt = m_tasks[regionCode].find(mapCode);
        if (mapIt != m_tasks[regionCode].end())
        {
            m_tasks[regionCode].insert(std::pair<std::string,
                std::map<std::string, std::map<std::string, std::string>>>());

            std::map<std::string, std::string> mapInfo;

            // Тут нужно загрузить для всех языков.
            std::string mapRussianTranslate = m_localization->getLocalization(mapCode);
            if (mapRussianTranslate == "none")
                mapRussianTranslate = m_localization->getLocalization(mapCode + "_NAME");
            if (mapRussianTranslate == "none")
                mapRussianTranslate = m_localization->getLocalization(mapCode + "_NEW_NAME");
            if (mapRussianTranslate == "none")
                mapRussianTranslate = m_localization->getLocalization("LEVEL_" + mapCode + "_NAME");
            if (mapRussianTranslate == "none")
                mapRussianTranslate = m_localization->getLocalization("LEVEL_" + mapCode);

            mapRussianTranslate = Utils::cutSlash(mapRussianTranslate);

            mapInfo.insert(std::pair<std::string, std::string>("rus", mapRussianTranslate));

            m_maps.insert(std::pair<std::string,
                std::map<std::string, std::map<std::string, std::string>>>(
                    regionCode, std::map<std::string,
                        std::map<std::string, std::string>>()));
            m_maps[regionCode].insert(std::pair<std::string,
                std::map<std::string, std::string>>(mapCode, mapInfo));
        }

        std::map<std::string, std::string> taskInfo;

        // Тут нужно загрузить для всех языков.
        std::string taskRussianTranslate = m_localization->getLocalization(taskCode);
        taskInfo.insert(std::pair<std::string, std::string>("rus", taskRussianTranslate));

        m_tasks[regionCode][mapCode].insert(std::pair<std::string,
            std::map<std::string, std::string>>(taskCode, taskInfo));
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

    rapidjson::Value regionsArray;
    regionsArray.SetObject();
    for (auto regionIt = m_regions.begin(); regionIt != m_regions.end(); regionIt++)
    {
        rapidjson::Value regionObject;
        regionObject.SetObject();

        rapidjson::Value regionCode;
        regionCode.SetString(regionIt->first.c_str(), regionIt->first.length(), allocator);

        for (std::pair<std::string, std::string> properties : regionIt->second)
        {
            rapidjson::Value propertyName;
            propertyName.SetString(properties.first.c_str(), properties.first.length(), allocator);

            rapidjson::Value propertyValue;
            propertyValue.SetString(properties.second.c_str(), properties.second.length(), allocator);

            regionObject.AddMember(propertyName, propertyValue, allocator);
        }

        regionsArray.AddMember(regionCode, regionObject, allocator);

    }
    databaseJsonDocument.AddMember("regions", regionsArray, allocator);

    rapidjson::Value mapsArray;
    mapsArray.SetObject();
    for (auto regionIt = m_maps.begin(); regionIt != m_maps.end(); regionIt++)
    {
        rapidjson::Value regionObject;
        regionObject.SetObject();

        rapidjson::Value regionCode;
        regionCode.SetString(regionIt->first.c_str(), regionIt->first.length(), allocator);

        for (auto mapsIt = m_maps[regionIt->first].begin(); mapsIt != m_maps[regionIt->first].end(); mapsIt++)
        {
            rapidjson::Value mapObject;
            mapObject.SetObject();

            rapidjson::Value mapCode;
            mapCode.SetString(mapsIt->first.c_str(), mapsIt->first.length(), allocator);

            for (std::pair<std::string, std::string> properties : mapsIt->second)
            {
                rapidjson::Value propertyName;
                propertyName.SetString(properties.first.c_str(), properties.first.length(), allocator);

                rapidjson::Value propertyValue;
                propertyValue.SetString(properties.second.c_str(), properties.second.length(), allocator);

                mapObject.AddMember(propertyName, propertyValue, allocator);
            }

            regionObject.AddMember(mapCode, mapObject, allocator);
        }
        mapsArray.AddMember(regionCode, regionObject, allocator);
    }
    databaseJsonDocument.AddMember("maps", mapsArray, allocator);

    rapidjson::StringBuffer writeBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(writeBuffer);
    databaseJsonDocument.Accept(writer);

    std::ofstream writeStream(_filename);
    writeStream << writeBuffer.GetString();
    writeStream.close();
}

std::map<std::string, std::map<std::string, std::map<std::string,
    std::map<std::string, std::string> > > >& Database::tasks()
{
    return m_tasks;
}

void Database::loadUpgradesData()
{
    // Read initial.cache_block.
    std::ifstream readStream(QDir::currentPath().toStdString() +
        "/database/initial.cache_block", std::ios::binary);
    std::stringstream buffer;
    buffer << readStream.rdbuf();
    readStream.close();
    std::string initial_cache_block_std = buffer.str();

    // Read upgrades.json.
    QFile file(QDir::currentPath() + "/jsons/upgrades.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString text = file.readAll();
    file.close();
    rapidjson::Document jsonDocument;
    jsonDocument.Parse(text.toUtf8());

    rapidjson::Value& persistentProfileData = jsonDocument["upgradesGiverData"];
    auto array = persistentProfileData.GetArray();
    for (int i = 0; i < array.Size(); i++)
    {
        auto levelArray = array[i].GetArray();
        for (int j = 0; j < levelArray.Size(); j++)
        {
            auto indexOfUpgrade = initial_cache_block_std.find(levelArray[j].GetString());
            if (indexOfUpgrade != std::string::npos)
            {
                auto indexOfUpgradeTranslateStart = initial_cache_block_std.find("[", indexOfUpgrade);
                auto indexOfUpgradeTranslateEnd = initial_cache_block_std.find("]", indexOfUpgradeTranslateStart);
                std::string upgradeTranslation = initial_cache_block_std.substr(
                    indexOfUpgradeTranslateStart, indexOfUpgradeTranslateEnd - indexOfUpgradeTranslateStart);
                upgradeTranslation.erase(std::remove(upgradeTranslation.begin(), upgradeTranslation.end(), ' '), upgradeTranslation.end());
                upgradeTranslation.erase(std::remove(upgradeTranslation.begin(), upgradeTranslation.end(), '\r'), upgradeTranslation.end());
                upgradeTranslation.erase(std::remove(upgradeTranslation.begin(), upgradeTranslation.end(), '\n'), upgradeTranslation.end());
                upgradeTranslation.erase(std::remove(upgradeTranslation.begin(), upgradeTranslation.end(), '['), upgradeTranslation.end());
                upgradeTranslation.erase(std::remove(upgradeTranslation.begin(), upgradeTranslation.end(), ']'), upgradeTranslation.end());
                upgradeTranslation.erase(std::remove(upgradeTranslation.begin(), upgradeTranslation.end(), '\"'), upgradeTranslation.end());
//                std::cout << m_localization->getLocalization(QString::fromStdString(upgradeTranslation)).toStdString() << std::endl;
                std::cout << levelArray[j].GetString() << " : " << upgradeTranslation << std::endl;
            }
            else
            {
//                std::cout << levelArray[j].GetString() << " : " << m_localization->getLocalization(levelArray[j].GetString()).toStdString() << std::endl;
            }
        }
    }

}
