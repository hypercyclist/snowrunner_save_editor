#include "GameAtlas.h"
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
#include <QDir>
#include "rapidxml/rapidxml.hpp"
#include <QDebug>
#include "Truck.h"

#include <fstream>
#include <sstream>
#include <iostream>

GameAtlas::GameAtlas() :
    m_regions(),
    m_trucks(),
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

    rapidjson::Value& ppdObject = jsonDocument;
    int savesCount = 4;
    for (int i = 0; i < savesCount; i++)
    {
        std::string completeSaveMemberName = "CompleteSave";
        if (i > 0) {
            completeSaveMemberName += std::to_string(i);
        }
        if (jsonDocument.HasMember(completeSaveMemberName.c_str())) {
            ppdObject = jsonDocument[completeSaveMemberName.c_str()]["SslValue"]["upgradesGiverData"];
        }
    }

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

    QDir xmlFolder(QDir::currentPath() + "/database/generator_materials/xml/classes/");
    QFileInfoList xmlFolderContent = xmlFolder.entryInfoList(
        QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);

    QDir addonsXmlFolder(QDir::currentPath() + "/database/generator_materials/xml/_dlc");
    QFileInfoList addonsXmlFolderContent = addonsXmlFolder.entryInfoList(
        QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);
    for (QFileInfo folder : addonsXmlFolderContent)
    {
        QDir addonXmlFolder(folder.absoluteFilePath() + "/classes/");
        xmlFolderContent += addonXmlFolder.entryInfoList(
            QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);
    }
//    qDebug() << xmlFolderContent;

    for (QFileInfo fileInfo : xmlFolderContent)
    {
        if (fileInfo.fileName() == "engines" || fileInfo.fileName() == "gearboxes"
            || fileInfo.fileName() == "suspensions")
        {
            QDir nestedXmlFolder(fileInfo.absoluteFilePath());
            QFileInfoList nestedXmlFolderContent = nestedXmlFolder.entryInfoList(
                QStringList(), QDir::Files | QDir::NoDotAndDotDot);
            for (QFileInfo nestedFileInfo : nestedXmlFolderContent)
            {

                std::ifstream readStream(nestedFileInfo.absoluteFilePath().toStdString(), std::ios::binary);
                std::stringstream buffer;
                buffer << readStream.rdbuf();
                readStream.close();
                std::string xmlFileText = buffer.str();

                rapidxml::xml_document<> xmlDocument;
                xmlDocument.parse<0>(const_cast<char*>(xmlFileText.c_str()));

                rapidxml::xml_node<> *unknownNode = nullptr;
                rapidxml::xml_node<> *nestedNode = nullptr;
                UpgradeType upgradeType = UpgradeType::UNKNOWN;

                unknownNode = xmlDocument.first_node("EngineVariants");
                if (unknownNode)
                {
                    nestedNode = unknownNode->first_node("Engine");
                    upgradeType = UpgradeType::ENGINE;
                }

                unknownNode = xmlDocument.first_node("GearboxVariants");
                if (unknownNode)
                {
                    nestedNode = unknownNode->first_node("Gearbox");
                    upgradeType = UpgradeType::GEARBOX;
                }

                unknownNode = xmlDocument.first_node("SuspensionSetVariants");
                if (unknownNode)
                {
                    nestedNode = unknownNode->first_node("SuspensionSet");
                    upgradeType = UpgradeType::SUSPENSION;
                }

                if (nestedNode)
                {
                    for (rapidxml::xml_node<> *node = nestedNode; node; node = node->next_sibling())
                    {
                        rapidxml::xml_attribute<> *nameAttribute = node->first_attribute("Name");
                        if (!nameAttribute) {
                            continue;
                        }
                        std::string middleCode = Utils::stolower(nameAttribute->value());
                        rapidxml::xml_node<> *nodeGameData = node->first_node("GameData");
                        rapidxml::xml_node<> *nodeUiDesc = nodeGameData->first_node("UiDesc");
                        std::string translationCode = nodeUiDesc->first_attribute("UiName")->value();

                        Upgrade* upgrade = nullptr;
                        bool found = false;
                        for (const auto& regionPair : m_regions)
                        {
                            for (const auto& mapPair : regionPair.second->maps())
                            {
                                for (const auto& upgradePair : mapPair.second->upgrades())
                                {
                                    if (Utils::stolower(upgradePair.second->middleCode()) == Utils::stolower(middleCode))
                                    {
                                        upgrade = upgradePair.second;
                                        found = true;
                                    }
                                    if (found) break;
                                }
                                if (found) break;
                            }
                            if (found) break;
                        }

                        if (!upgrade) {
                            continue;
                        }

                        upgrade->setType(upgradeType);

                        for (const auto& languagePair : m_localization->languageTextNames())
                        {
                            std::string translation = m_localization->getLocalization(translationCode, languagePair.first);
                            upgrade->setName(languagePair.first, translation);

                            m_localization->getLocalization()[languagePair.first][middleCode] = translation;
                            m_localization->getLocalizationCache()[languagePair.first][middleCode] = translation;
                        }
                    }
                }
            }
        }
        else if (fileInfo.fileName() == "trucks")
        {
            QFileInfoList trucksFolderContent = QDir(fileInfo.absoluteFilePath()).entryInfoList(
                QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);
            for (QFileInfo tuningFolder : trucksFolderContent)
            {
                QFileInfoList tuningFolderContent = QDir(tuningFolder.absoluteFilePath()).entryInfoList(
                    QStringList(), QDir::Files | QDir::NoDotAndDotDot);

                for (QFileInfo tuningFileInfo : tuningFolderContent)
                {
                    std::string tuningFilePath = tuningFileInfo.absoluteFilePath().toStdString();
                    if (Utils::contains(tuningFilePath, "diff_lock.xml") || Utils::contains(tuningFilePath, "transferbox_allwheels.xml"))
                    {
                        std::ifstream readStream(tuningFilePath);
                        if (!readStream.is_open())
                        {
                            qDebug() << readStream.bad();
                        }
                        std::stringstream buffer;
                        buffer << readStream.rdbuf();
                        readStream.close();
                        std::string xmlFileText = buffer.str();

                        rapidxml::xml_document<> xmlDocument;
                        xmlDocument.parse<0>(const_cast<char*>(xmlFileText.c_str()));

                        std::string translationCode = xmlDocument.first_node("TruckAddon")->first_node("GameData")->first_node("UiDesc")->first_attribute("UiName")->value();

                        std::string middleCode = tuningFileInfo.fileName().toStdString().substr(0, tuningFileInfo.fileName().toStdString().length() - 4);
                        Upgrade* upgrade = nullptr;
                        bool found = false;
                        for (const auto& regionPair : m_regions)
                        {
                            for (const auto& mapPair : regionPair.second->maps())
                            {
                                for (const auto& upgradePair : mapPair.second->upgrades())
                                {
                                    if (Utils::stolower(upgradePair.second->middleCode()) == Utils::stolower(middleCode))
                                    {
                                        upgrade = upgradePair.second;
                                        found = true;
                                    }
                                    if (found) break;
                                }
                                if (found) break;
                            }
                            if (found) break;
                        }

                        if (!upgrade) {
                            continue;
                        }

                        if (Utils::contains(tuningFilePath, "diff_lock.xml")) {
                            upgrade->setType(UpgradeType::DIFFLOCK);
                        }
                        else if (Utils::contains(tuningFilePath, "transferbox_allwheels.xml")) {
                            upgrade->setType(UpgradeType::TRANSFERBOX);
                        }


                        for (const auto& languagePair : m_localization->languageTextNames())
                        {
                            std::string translation = m_localization->getLocalization(translationCode, languagePair.first);
                            upgrade->setName(languagePair.first, translation);

                            m_localization->getLocalization()[languagePair.first][upgrade->middleCode()] = translation;
                            m_localization->getLocalizationCache()[languagePair.first][upgrade->middleCode()] = translation;
                        }
                    }
                }
            }
        }
    }
}

void GameAtlas::createTrucksData()
{
    QDir xmlFolder(QDir::currentPath() + "/database/generator_materials/xml/classes/");
    QFileInfoList xmlFolderContent = xmlFolder.entryInfoList(
        QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);

    QDir addonsXmlFolder(QDir::currentPath() + "/database/generator_materials/xml/_dlc");
    QFileInfoList addonsXmlFolderContent = addonsXmlFolder.entryInfoList(
        QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);
    for (QFileInfo folder : addonsXmlFolderContent)
    {
        QDir addonXmlFolder(folder.absoluteFilePath() + "/classes/");
        xmlFolderContent += addonXmlFolder.entryInfoList(
            QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);
    }

    for (QFileInfo fileInfo : xmlFolderContent)
    {
        if (!fileInfo.isDir())
        {
            continue;
        }

        if (fileInfo.fileName() != "trucks")
        {
            continue;
        }

        QDir nestedXmlFolder(fileInfo.absoluteFilePath());
        QFileInfoList nestedXmlFolderContent = nestedXmlFolder.entryInfoList(
            QStringList(), QDir::Files | QDir::NoDotAndDotDot);
        for (QFileInfo nestedFileInfo : nestedXmlFolderContent)
        {
            qDebug() << nestedFileInfo.absoluteFilePath();

            if (nestedFileInfo.fileName() == "zikz_5368.xml")
            {
                qDebug() << "zikz_5368.xml";
            }

            std::ifstream readStream(nestedFileInfo.absoluteFilePath().toStdString(), std::ios::binary);
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

            std::string truckCode = nestedFileInfo.fileName().toStdString().substr(0, nestedFileInfo.fileName().toStdString().length() - 4);
            std::string translationCode = "";
            rapidxml::xml_node<>* gameDataNode = node->first_node("GameData");
            if (gameDataNode)
            {
                rapidxml::xml_node<>* uiDescNode = gameDataNode->first_node("UiDesc");
                rapidxml::xml_attribute<>* uiDescAttribute = uiDescNode->first_attribute("UiName");
                if (uiDescAttribute) {
                    translationCode = uiDescAttribute->value();
                }
                else {
                    translationCode = uiDescNode->first_node("region:default")->first_attribute("UiName")->value();
                }
            }

            Truck* truck = new Truck(truckCode);

            truck->setNameCode(translationCode);
            for (const auto& languagePair : m_localization->languageTextNames())
            {
                truck->setName(languagePair.first, m_localization->getLocalization(translationCode, languagePair.first));
            }

            std::multimap<std::string, std::string> files;

            rapidxml::xml_node<> *truckDataNode = node->first_node("TruckData");
            if (truckDataNode)
            {
                rapidxml::xml_node<> *engineSocketNode = truckDataNode->first_node("EngineSocket");
                if (engineSocketNode) {
                    std::string typeText = engineSocketNode->first_attribute("Type")->value();
                    typeText = Utils::replace(typeText, " ", "");
                    for (std::string file : Utils::split(typeText, ','))
                    {
                        files.insert({"engines", file});
                    }
                }

                rapidxml::xml_node<> *gearboxSocketNode = truckDataNode->first_node("GearboxSocket");
                if (gearboxSocketNode) {
                    std::string typeText = gearboxSocketNode->first_attribute("Type")->value();
                    typeText = Utils::replace(typeText, " ", "");
                    for (std::string file : Utils::split(typeText, ','))
                    {
                        files.insert({"gearboxes", file});
                    }
                }

                rapidxml::xml_node<> *suspensionSocketNode = truckDataNode->first_node("SuspensionSocket");
                if (suspensionSocketNode) {
                    std::string typeText = suspensionSocketNode->first_attribute("Type")->value();
                    typeText = Utils::replace(typeText, " ", "");
                    for (std::string file : Utils::split(typeText, ','))
                    {
                        files.insert({"suspensions", file});
                    }
                }
            }

            for(auto filesIt = files.begin(); filesIt != files.end(); filesIt++)
            {
                std::string filePathString = QDir::currentPath().toStdString()
                                             + "/database/generator_materials/xml/classes/" + filesIt->first + "/"
                                             + filesIt->second + ".xml";
                qDebug() << QString::fromStdString(filePathString);
            }

            for(auto filesIt = files.begin(); filesIt != files.end(); filesIt++)
            {
                QFileInfo file;
                std::string filePathString = QDir::currentPath().toStdString()
                    + "/database/generator_materials/xml/classes/" + filesIt->first + "/"
                    + filesIt->second + ".xml";

                qDebug() << QString::fromStdString(filePathString);
                if (Utils::contains(filePathString, "e_us_truck_modern_longhorn_4520.xml"))
                {
                    std::cout << "here";
                }

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
                    if (!upgradeVariantsNode)
                    {
                        std::cout << "here";
                    }
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
    for (QFileInfo fileInfo : xmlFolderContent)
    {
        if (!fileInfo.isDir())
        {
            continue;
        }

        if (fileInfo.fileName() != "trucks")
        {
            continue;
        }
        QDir nestedXmlFolder(fileInfo.absoluteFilePath());
        QFileInfoList nestedXmlFolderContent = nestedXmlFolder.entryInfoList(
            QStringList(), QDir::Files | QDir::NoDotAndDotDot);
        QFileInfoList nestedTunningFoldersContent = nestedXmlFolder.entryInfoList(
            QStringList() << "*_tuning", QDir::Dirs | QDir::NoDotAndDotDot);
        for (QFileInfo nestedFileInfo : nestedTunningFoldersContent)
        {
            std::string folderName = nestedFileInfo.fileName().toStdString();
            std::string truckMiddleCode = Utils::replace(folderName, "_tuning", "");
            Truck* truck = nullptr;
            for (const auto& truckPair : m_trucks)
            {
                if (truckPair.first == truckMiddleCode)
                {
                    truck = truckPair.second;
                }
            }

            if (!truck) {
                continue;
            }

            QFileInfoList tuningFolderContent = QDir(nestedFileInfo.absoluteFilePath()).entryInfoList(
                QStringList(), QDir::Files | QDir::NoDotAndDotDot);
            for (QFileInfo tuningFileInfo : tuningFolderContent)
            {
                std::string tuningFilePath = tuningFileInfo.absoluteFilePath().toStdString();
                if (Utils::contains(tuningFilePath, "diff_lock.xml"))
                {
                    std::string middleCode = tuningFileInfo.fileName().toStdString().substr(0, tuningFileInfo.fileName().toStdString().length() - 4);
                    truck->addUpgrade(middleCode);
                }
                if (Utils::contains(tuningFilePath, "allwheels.xml"))
                {
                    std::string middleCode = tuningFileInfo.fileName().toStdString().substr(0, tuningFileInfo.fileName().toStdString().length() - 4);
                    truck->addUpgrade(middleCode);
                }
            }
        }
    }
}

void GameAtlas::connectUpgradesWithTrucks()
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

                for (const auto& truckPair : m_trucks)
                {
                    std::vector<std::string> truckUpgrades = truckPair.second->upgrades();
                    if (std::find(truckUpgrades.begin(), truckUpgrades.end(), upgrade->middleCode()) != truckUpgrades.end() )
                    {
                        upgrade->addTruck(truckPair.second->nameCode());
                    }
                }
            }
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

                upgrade->setType(static_cast<UpgradeType>(upgradeIt->value["type"].GetInt()));
                upgrade->setMiddleCode(upgradeIt->value["middleCode"].GetString());

                for (const auto& truck : upgradeIt->value["upgradableTrucks"].GetArray())
                {
                    upgrade->addTruck(truck.GetString());
                }

                for (const auto& languagePair : m_localization->languageTextNames())
                {
                    std::string translation = m_localization->getLocalization(upgrade->middleCode(), languagePair.first);
                    translation = translation == "none" ? upgrade->middleCode() : translation;
                    upgrade->setName(languagePair.first, translation);
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
    rapidjson::Value& trucksObject = tasksJsonDocument["Trucks"];
    for (auto trucksIt = trucksObject.MemberBegin(); trucksIt != trucksObject.MemberEnd(); trucksIt++)
    {
        Truck* truck = new Truck(trucksIt->name.GetString());

        truck->setNameCode(trucksIt->value["nameCode"].GetString());

        for (const auto& languagePair : m_localization->languageTextNames())
        {
            std::string translation = m_localization->getLocalization(truck->nameCode(), languagePair.first);
            truck->setName(languagePair.first, translation);
        }

        for (auto& upgradeCodeObject : trucksIt->value["truckUpgrades"].GetArray())
        {
            truck->addUpgrade(upgradeCodeObject.GetString());
        }
        m_trucks[truck->code()] = truck;
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

                rapidjson::Value upgradeType;
                upgradeType.SetInt(static_cast<int>(upgrade->type()));
                upgradeObject.AddMember("type", upgradeType, allocator);

                rapidjson::Value upgradeMiddleCode;
                upgradeMiddleCode.SetString(upgrade->middleCode().c_str(), upgrade->middleCode().length(), allocator);
                upgradeObject.AddMember("middleCode", upgradeMiddleCode, allocator);

                rapidjson::Value upgradableTrucks;
                upgradableTrucks.SetArray();
                for (std::string truckCode : upgrade->trucks())
                {
                    rapidjson::Value truckCodeStringObject;
                    truckCodeStringObject.SetString(truckCode.c_str(), truckCode.length(), allocator);

                    upgradableTrucks.PushBack(truckCodeStringObject, allocator);
                }
                upgradeObject.AddMember("upgradableTrucks", upgradableTrucks, allocator);

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

    rapidjson::Value trucksObject;
    trucksObject.SetObject();
    for (const auto& truckPair : m_trucks)
    {
        Truck* truck = truckPair.second;

        rapidjson::Value truckObject;
        truckObject.SetObject();

        rapidjson::Value truckCode;
        truckCode.SetString(truck->code().c_str(), truck->code().length(), allocator);

        rapidjson::Value truckNameCode;
        truckNameCode.SetString(truck->nameCode().c_str(), truck->nameCode().length(), allocator);
        truckObject.AddMember("nameCode", truckNameCode, allocator);

        rapidjson::Value truckUpgrades;
        truckUpgrades.SetArray();
        for (std::string upgradeCode : truck->upgrades())
        {
            rapidjson::Value upgradeCodeStringObject;
            upgradeCodeStringObject.SetString(upgradeCode.c_str(), upgradeCode.length(), allocator);

            truckUpgrades.PushBack(upgradeCodeStringObject, allocator);
        }
        truckObject.AddMember("truckUpgrades", truckUpgrades, allocator);

        trucksObject.AddMember(truckCode, truckObject, allocator);
    }

    if (databaseJsonDocument.HasMember("Trucks"))
    {
        databaseJsonDocument["Trucks"] = trucksObject;
    }
    else
    {
        databaseJsonDocument.AddMember("Trucks", trucksObject, allocator);
    }

    rapidjson::StringBuffer writeBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(writeBuffer);
    databaseJsonDocument.Accept(writer);

    std::ofstream writeStream(_filename  + "/database.json");
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
