#include "SaveFile.h"

#include "Utils.h"
#include "rapidjson/writer.h"

#include <fstream>
#include <sstream>

SaveFile::SaveFile() :
    m_loaded(false),
    m_error(),
    saveFileJsonDocument(),
    completeSaveJMN(),
    m_money(0),
    m_rank(0),
    m_experience(0),
    m_finishedObjs(),
    m_foundedUpgrades(),
    m_receivedUpgrades()
{ }

SaveFile::SaveFile(std::string _fullFileName) :
    SaveFile()
{
    loadSaveFile(_fullFileName);
}

SaveFile::~SaveFile()
{ }

// JMN is json member name.
// JMP is json member pointer.
// JMO is json member object.
// JMA is json member array.
bool SaveFile::loadSaveFile(std::string _fullFileName)
{
// Read file to buffer, then to string, then parse it as JSON file.
    std::ifstream readStream(_fullFileName);
    if (!readStream.is_open())
    {
        m_error = "Can't open file " + _fullFileName;
        return false;
    }

    std::stringstream buffer;
    buffer << readStream.rdbuf();
    readStream.close();
    std::string saveFileString = buffer.str();
    saveFileJsonDocument.Parse(saveFileString.c_str());

    if (saveFileString.size() == 0)
    {
        m_error = "File " + _fullFileName + " is empty";
        return false;
    }

    if (!saveFileJsonDocument.IsObject())
    {
        m_error = "File " + _fullFileName + " is not JSON valid or broken";
        return false;
    }

// Search for save number.
    rapidjson::Value* completeSaveJMP = nullptr;
    int savesMaxCount = 12;
    for (int i = 0; i < savesMaxCount; i++)
    {
        completeSaveJMN = i > 0 ?
            "CompleteSave" + std::to_string(i) : "CompleteSave";

        if (saveFileJsonDocument.HasMember(completeSaveJMN.c_str()))
        {
            completeSaveJMP = &saveFileJsonDocument[completeSaveJMN.c_str()];
            break;
        }
    }
// Checking JSON is OK. Read values.
    if (!completeSaveJMP)
    {
        m_error = "Can't find member of JSON named \"CompleteSave\"";
        return false;
    }
    rapidjson::Value& completeSaveJMO = *completeSaveJMP;

    if (!completeSaveJMO.HasMember("SslValue"))
    {
        m_error = "Can't find member of JSON named \"SslValue\"";
        return false;
    }
    rapidjson::Value& sslValueJMO = completeSaveJMO["SslValue"];

    if (!sslValueJMO.HasMember("persistentProfileData"))
    {
        m_error = "Can't find member of JSON named \"persistentProfileData\"";
        return false;
    }
    rapidjson::Value& persistentProfileDataJMO = sslValueJMO["persistentProfileData"];

    m_money = persistentProfileDataJMO["money"].GetInt();
    m_rank = persistentProfileDataJMO["rank"].GetInt();
    m_experience = persistentProfileDataJMO["experience"].GetInt();

    m_gameDifficultyMode = sslValueJMO.HasMember("gameDifficultyMode") ?
        sslValueJMO["gameDifficultyMode"].GetInt() : 0;

    m_isHardMode = sslValueJMO["isHardMode"].GetBool();

    rapidjson::Value& finishedObjsJMA = sslValueJMO["finishedObjs"].GetArray();
    for (const auto& element : finishedObjsJMA.GetArray())
    {
        m_finishedObjs.push_back(element.GetString());
    }

    rapidjson::Value& upgradesGiverDataJMA = sslValueJMO["upgradesGiverData"];
    for (auto mapIt = upgradesGiverDataJMA.MemberBegin(); mapIt != upgradesGiverDataJMA.MemberEnd(); mapIt++)
    {
        for (auto upgradesIt = mapIt->value.MemberBegin(); upgradesIt != mapIt->value.MemberEnd(); upgradesIt++)
        {
            switch(upgradesIt->value.GetInt())
            {
            case 1: m_foundedUpgrades.push_back(upgradesIt->name.GetString());
            case 2: m_receivedUpgrades.push_back(upgradesIt->name.GetString());
            }
        }
    }

    m_loaded = true;
    return true;
}

bool SaveFile::loaded()
{
    return m_loaded;
}

bool SaveFile::saveSaveFile(std::string _fullFileName)
{
    auto& allocator = saveFileJsonDocument.GetAllocator();

    rapidjson::Value& sslValueJMO = saveFileJsonDocument[completeSaveJMN.c_str()]["SslValue"];
    rapidjson::Value& persistentProfileDataJMO = sslValueJMO["persistentProfileData"];
    persistentProfileDataJMO["money"] = m_money;
    persistentProfileDataJMO["rank"] = m_rank;
    persistentProfileDataJMO["experience"] = m_experience;

    if (sslValueJMO.HasMember("gameDifficultyMode"))
    {
        sslValueJMO["gameDifficultyMode"] = m_gameDifficultyMode;
    }
    else
    {
        sslValueJMO.AddMember("gameDifficultyMode", m_gameDifficultyMode, allocator);
    }

    sslValueJMO["isHardMode"] = m_isHardMode;

    rapidjson::Value finishedObjsJMA;
    finishedObjsJMA.SetArray();
    for (std::string element : m_finishedObjs)
    {
        rapidjson::Value value(element.c_str(), element.length(), allocator);
        finishedObjsJMA.PushBack(value, allocator);
    }
    sslValueJMO["finishedObjs"] = finishedObjsJMA;

    rapidjson::Value discoveredObjectivesJMA;
    discoveredObjectivesJMA.SetArray();
    for (std::string element : m_finishedObjs) // It's not an error!
    {
        rapidjson::Value value(element.c_str(), element.length(), allocator);
        discoveredObjectivesJMA.PushBack(value, allocator);
    }
    sslValueJMO["discoveredObjectives"] = discoveredObjectivesJMA;

// Override upgrades. If we override value it's in priority. Next priority is
// save founded but didn't received upgrades. Then if it's not founded set 0.
    rapidjson::Value& upgradesGiverDataJMO = sslValueJMO["upgradesGiverData"];
    for (auto& levelJMP : upgradesGiverDataJMO.GetObject())
    {
        for (auto& upgradeJMP : levelJMP.value.GetObject())
        {
            bool found = false;
            if (Utils::contains(m_receivedUpgrades, upgradeJMP.name.GetString()))
            {
                upgradeJMP.value = 2;
                found = true;
            }
            if (!found)
            {
                if (Utils::contains(m_finishedObjs, upgradeJMP.name.GetString()))
                {
                    upgradeJMP.value = 1;
                    found = true;
                }
                if (!found)
                {
                    upgradeJMP.value = 0;
                }
            }
        }
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    saveFileJsonDocument.Accept(writer);

    std::ofstream file(_fullFileName);
    if (!file.is_open())
    {
        m_error = "Can't open/create file: " + _fullFileName;
        return false;
    }

    file << buffer.GetString() << '\0';
    file.close();

    return true;
}

std::string SaveFile::error()
{
    return m_error;
}

// Fuck this shit. I'm out.
//int SaveFile::money()
//{
//    return m_money;
//}

//void SaveFile::setMoney(int _value)
//{
//    m_money = _value;
//}

//int SaveFile::rank()
//{
//    return m_rank;
//}

//void SaveFile::setRank(int _value)
//{
//    m_rank = _value;
//}

//int SaveFile::experience()
//{
//    return m_experience;
//}

//void SaveFile::setExperience(int _value)
//{
//    m_experience = _value;
//}

//int SaveFile::gameDifficultyMode()
//{
//    return m_gameDifficultyMode;
//}

//void SaveFile::setGameDifficultyMode(int _value)
//{
//    m_gameDifficultyMode = _value;
//}

//int SaveFile::isHardMode()
//{
//    return m_isHardMode;
//}

//void SaveFile::setIsHardMode(int _value)
//{
//    m_isHardMode = _value;
//}

//std::vector<std::string> SaveFile::finishedObjs()
//{
//    return m_finishedObjs;
//}

//void SaveFile::setFinishedObjs(std::vector<std::string> _value)
//{
//    m_finishedObjs = _value;
//}

//std::vector<std::string> SaveFile::foundedUpgrades()
//{
//    return m_foundedUpgrades;
//}

//void SaveFile::setFoundedUpgrades(std::vector<std::string> _value)
//{
//    m_foundedUpgrades = _value;
//}

//std::vector<std::string> SaveFile::receivedUpgrades()
//{
//    return m_receivedUpgrades;
//}

//void SaveFile::setReceivedUpgrades(std::vector<std::string> _value)
//{
//    m_receivedUpgrades = _value;
//}
