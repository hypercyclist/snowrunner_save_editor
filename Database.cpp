#include "Database.h"
#include <QDir>
#include <iostream>
#include "Localization.h"
#include "GameAtlas.h"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include "rapidxml/rapidxml.hpp"
//#include "NewGamePlusSettings.h"
#include <fstream>
#include <sstream>
#include "Region.h"
#include "Map.h"
#include "Task.h"
#include "Truck.h"
#include "Utils.h"
#include <QDebug>

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

// Read regions, maps, tasks.
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
            for (const auto& languagePair : m_localization->languageTextNames())
            {
                std::string translation = m_localization->localization(regionCode, languagePair.first);
                translation = Utils::cutLongCountryName(translation);
                region->setName(languagePair.first, translation);
            }
            region->setTranslationCode(regionCode);
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
        task->setTranslationCode(taskCode);
        task->setRegionCode(regionCode);
        task->setMapCode(mapCode);

        for (const auto& languagePair : m_localization->languageTextNames())
        {
            std::string translation = m_localization->localization(taskCode, languagePair.first);
            task->setName(languagePair.first, translation);
        }

        map->addTask(task);
    }

// Read vehicles.
    QString rootXmlFolderPath = QString::fromStdString(_databasePath) + "/generator_materials/xml";

    QFileInfoList joinedGameClassesFoldersContent = QDir(rootXmlFolderPath + "/classes")
        .entryInfoList(QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);

    QFileInfoList joinedTruckTuningFoldersContent;

    QFileInfoList dlcFolderContent = QDir(rootXmlFolderPath + "/_dlc")
        .entryInfoList(QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);
    for (QFileInfo specificDlcFolder : dlcFolderContent)
    {
        joinedGameClassesFoldersContent += QDir(specificDlcFolder.absoluteFilePath() + "/classes")
            .entryInfoList(QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);
    }

    for (QFileInfo classFolderInfo : joinedGameClassesFoldersContent)
    {
        if (classFolderInfo.fileName() == "trucks")
        {
            QFileInfoList classFolderContent = QDir(classFolderInfo.absoluteFilePath())
                .entryInfoList(QStringList(), QDir::Files | QDir::NoDotAndDotDot);

            for (QFileInfo fileFileInfo : classFolderContent)
            {
                std::ifstream readStream(fileFileInfo.absoluteFilePath().toStdString(), std::ios::binary);
                std::stringstream buffer;
                buffer << readStream.rdbuf();
                readStream.close();
                std::string xmlFileText = buffer.str();

                rapidxml::xml_document<> xmlDocument;
                xmlDocument.parse<0>(const_cast<char*>(xmlFileText.c_str()));

                rapidxml::xml_node<> *node = xmlDocument.first_node("Truck");

                if (!node) {
                    continue;
                }

                std::string truckCode = fileFileInfo.fileName().toStdString().substr(0, fileFileInfo.fileName().toStdString().length() - 4);
                std::string translationCode = "";
                rapidxml::xml_node<> *gameDataNode = node->first_node("GameData");
                if (gameDataNode)
                {
                    translationCode = gameDataNode->first_node("UiDesc")->first_attribute("UiName")->value();
                }

                Truck* truck = new Truck(truckCode);

                truck->setNameCode(translationCode); // check.
                truck->setTranslationCode(translationCode);
                for (const auto& languagePair : m_localization->languageTextNames())
                {
                    truck->setName(languagePair.first, m_localization->localization(translationCode, languagePair.first));
                }

                rapidxml::xml_node<> *truckDataNode = node->first_node("TruckData");
                if (truckDataNode)
                {
                    rapidxml::xml_node<> *engineSocketNode = truckDataNode->first_node("EngineSocket");
                    if (engineSocketNode) {
                        std::string typeText = engineSocketNode->first_attribute("Type")->value();
                        typeText = Utils::replace(typeText, " ", "");
                        for (std::string file : Utils::split(typeText, ','))
                        {
                            truck->addEngineSocketType(file);
                        }
                    }

                    rapidxml::xml_node<> *gearboxSocketNode = truckDataNode->first_node("GearboxSocket");
                    if (gearboxSocketNode) {
                        std::string typeText = gearboxSocketNode->first_attribute("Type")->value();
                        typeText = Utils::replace(typeText, " ", "");
                        for (std::string file : Utils::split(typeText, ','))
                        {
                            truck->addGearboxSocketType(file);
                        }
                    }

                    rapidxml::xml_node<> *suspensionSocketNode = truckDataNode->first_node("SuspensionSocket");
                    if (suspensionSocketNode) {
                        std::string typeText = suspensionSocketNode->first_attribute("Type")->value();
                        typeText = Utils::replace(typeText, " ", "");
                        for (std::string file : Utils::split(typeText, ','))
                        {
                            truck->addSuspensionSocketType(file);
                        }
                    }
                }

                auto socketTypes = truck->socketTypes();
                for(auto filesIt = socketTypes.begin(); filesIt != socketTypes.end(); filesIt++)
                {
                    QFileInfo file;
                    std::string filePathString = rootXmlFolderPath.toStdString() + "/" + filesIt->first + "/"
                        + filesIt->second + ".xml";
                    if (QFileInfo::exists(QString::fromStdString(filePathString)))
                    {
                        file.setFile(QString::fromStdString(filePathString));
                    }
                    else
                    {
                        QDir addonsXmlFolder(QDir::currentPath() + "/database/generator_materials/xml/_dlc");
                        QFileInfoList addonsXmlFolderContent = addonsXmlFolder.entryInfoList(
                            QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);
                        for (QFileInfo folder : addonsXmlFolderContent)
                        {
                            std::string filePathString = folder.absoluteFilePath().toStdString() + "/classes/" + filesIt->first + "/" + filesIt->second + ".xml";
                            if (QFileInfo::exists(QString::fromStdString(filePathString)))
                            {
                                file.setFile(QString::fromStdString(filePathString));
                            }
                        }
                    }

                    std::ifstream readStream(file.absoluteFilePath().toStdString(), std::ios::binary);
                    std::stringstream buffer;
                    buffer << readStream.rdbuf();
                    readStream.close();
                    std::string xmlFileText = buffer.str();

                    rapidxml::xml_document<> xmlDocument;
                    xmlDocument.parse<0>(const_cast<char*>(xmlFileText.c_str()));

                    rapidxml::xml_node<> *upgradeVariantsNode = nullptr;
                    rapidxml::xml_node<> *upgradeNode = nullptr;
                    if (filesIt->first == "engines")
                    {
                        upgradeVariantsNode = xmlDocument.first_node("EngineVariants");
                        upgradeNode = upgradeVariantsNode->first_node("Engine");
                    }
                    else if (filesIt->first == "gearboxes")
                    {
                        upgradeVariantsNode = xmlDocument.first_node("GearboxVariants");
                        upgradeNode = upgradeVariantsNode->first_node("Gearbox");
                    }
                    else if (filesIt->first == "suspensions")
                    {
                        upgradeVariantsNode = xmlDocument.first_node("SuspensionSetVariants");
                        upgradeNode = upgradeVariantsNode->first_node("SuspensionSet");
                    }

                    if (upgradeVariantsNode)
                    {
                        for (rapidxml::xml_node<> *node = upgradeNode; node; node = node->next_sibling())
                        {
                            rapidxml::xml_attribute<> *nameAttribute = node->first_attribute("Name");
                            if (!nameAttribute) {
                                continue;
                            }
                            truck->addUpgrade(nameAttribute->value());
                        }
                    }
                }

                m_trucks[truckCode] = truck;
            }

        }
        else
        {
//            parseUpgrade();
        }
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
