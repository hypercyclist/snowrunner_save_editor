#include "AppConfig.h"

#include "rapidjson/writer.h"

#include <QDir>
#include <iostream>
#include <fstream>
#include <sstream>

AppConfig::AppConfig() :
    m_loaded(false),
    m_error(),
    configFileJsonDocument(),
    m_configFullFileName(),
    m_databaseFolderPath(QDir::currentPath().toStdString() + "/database/"),
    m_lastSaveFolderPath(),
    m_lastLanguageCode("russian")
{ }

AppConfig::AppConfig(std::string _fullFileName) :
    AppConfig()
{
    loadConfig(_fullFileName);
}

AppConfig::~AppConfig()
{

}

bool AppConfig::loadConfig(std::string _fullFileName)
{
    m_configFullFileName = _fullFileName;

    std::ifstream readStream(_fullFileName);
    if (!readStream.is_open())
    {
        std::cout << "Can't open config file: " + _fullFileName << std::endl;
        std::cout << "It will be created." << std::endl;
        if (!saveConfig(_fullFileName))
        {
            std::cout << "Can't save config file: " + _fullFileName << std::endl;
            std::cout << "You should check you able to write files in app folder!" << std::endl;
            return false;
        }
    }

    readStream.open(_fullFileName);
    std::stringstream buffer;
    buffer << readStream.rdbuf();
    readStream.close();
    std::string saveFileString = buffer.str();
    configFileJsonDocument.Parse(saveFileString.c_str());

    if (configFileJsonDocument.HasMember("databaseFolderPath"))
    {
        m_databaseFolderPath = configFileJsonDocument["databaseFolderPath"].GetString();
    }

    if (configFileJsonDocument.HasMember("lastSaveFolderPath"))
    {
        m_lastSaveFolderPath = configFileJsonDocument["lastSaveFolderPath"].GetString();
    }

    if (configFileJsonDocument.HasMember("lastLanguageCode"))
    {
        m_lastLanguageCode = configFileJsonDocument["lastLanguageCode"].GetString();
    }

    m_loaded = true;
    return true;
}

bool AppConfig::loaded()
{
    return m_loaded;
}

bool AppConfig::saveConfig()
{
    saveConfig(m_configFullFileName);
}

bool AppConfig::saveConfig(std::string _fullFileName)
{
    auto& allocator = configFileJsonDocument.GetAllocator();

    configFileJsonDocument.SetObject();

    if (configFileJsonDocument.HasMember("databaseFolderPath"))
    {
        configFileJsonDocument["databaseFolderPath"].SetString(m_databaseFolderPath.c_str(), m_databaseFolderPath.length());
    }
    else
    {
        rapidjson::Value databaseFolderPathValue(m_databaseFolderPath.c_str(),
            m_databaseFolderPath.length(), allocator);
        configFileJsonDocument.AddMember("databaseFolderPath",
            databaseFolderPathValue, allocator);
    }

    if (configFileJsonDocument.HasMember("lastSaveFolderPath"))
    {
        configFileJsonDocument["lastSaveFolderPath"].SetString(m_lastSaveFolderPath.c_str(), m_lastSaveFolderPath.length());
    }
    else
    {
        rapidjson::Value lastSaveFolderPathValue(m_lastSaveFolderPath.c_str(),
            m_lastSaveFolderPath.length(), allocator);
        configFileJsonDocument.AddMember("lastSaveFolderPath",
            lastSaveFolderPathValue, allocator);
    }

    if (configFileJsonDocument.HasMember("lastLanguageCode"))
    {
        configFileJsonDocument["lastLanguageCode"].SetString(m_lastLanguageCode.c_str(), m_lastLanguageCode.length());
    }
    else
    {
        rapidjson::Value lastLanguageCodeValue(m_lastLanguageCode.c_str(),
            m_lastLanguageCode.length(), allocator);
        configFileJsonDocument.AddMember("lastLanguageCode",
            lastLanguageCodeValue, allocator);
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    configFileJsonDocument.Accept(writer);

    std::ofstream file(_fullFileName);
    if (!file.is_open())
    {
        m_error = "Can't open/create file: " + _fullFileName;
        return false;
    }

    file << buffer.GetString();
    file.close();

    return true;
}

std::string AppConfig::configFullFileName()
{
    return m_configFullFileName;
}

std::string AppConfig::databaseFolderPath()
{
    return m_databaseFolderPath;
}

void AppConfig::setDatabaseFolderPath(std::string _path)
{
    m_databaseFolderPath = _path;
}


std::string AppConfig::lastSaveFolderPath()
{
    return m_lastSaveFolderPath;
}

void AppConfig::setLastSaveFolderPath(std::string _path)
{
    m_lastSaveFolderPath = _path;
}


std::string AppConfig::lastLanguageCode()
{
    return m_lastLanguageCode;
}

void AppConfig::setLastLanguageCode(std::string _languageCode)
{
    m_lastLanguageCode = _languageCode;
}
