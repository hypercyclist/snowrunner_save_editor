#ifndef APPCONFIG_H
#define APPCONFIG_H

#include "rapidjson/document.h"

#include <string>

class AppConfig
{
public:
    AppConfig();
    AppConfig(std::string _fullFileName);
    ~AppConfig();

    bool loadConfig(std::string _fullFileName);
    bool loaded();
    bool saveConfig();
    bool saveConfig(std::string _fullFileName);

    std::string configFullFileName();

    std::string databaseFolderPath();
    void setDatabaseFolderPath(std::string _path);

    std::string lastSaveFolderPath();
    void setLastSaveFolderPath(std::string _path);

    std::string lastLanguageCode();
    void setLastLanguageCode(std::string _languageCode);

private:
    bool m_loaded;
    std::string m_error;
    rapidjson::Document configFileJsonDocument;
    std::string m_configFullFileName;
    std::string m_databaseFolderPath;
    std::string m_lastSaveFolderPath;
    std::string m_lastLanguageCode;


};

#endif // APPCONFIG_H
