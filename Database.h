#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <map>
#include <vector>

class Localization;

/*
 * Regions: rus, eng
 * Maps: rus, eng
 * Tasks: rus, eng
 * Upgrades: realCode, rus, eng
*/

class Database
{
public:
    Database();
    void createDatabase();
    void loadDatabase();
    std::map<std::string, std::map<std::string, std::string>>& regions();

    std::map<std::string, std::map<std::string,
        std::map<std::string, std::string>>>& maps();

    std::map<std::string, std::map<std::string, std::map<std::string,
        std::map<std::string, std::string>>>>& tasks();
private:
    Localization* m_localization;

    // Region - Transcriptions.
    std::map<std::string,
        std::map<std::string, std::string>> m_regions;

    // Region, Map - Transcriptions.
    std::map<std::string,
        std::map<std::string,
            std::map<std::string, std::string>>> m_maps;

    // Region, Map, Task - Transcriptions.
    std::map<std::string,
        std::map<std::string,
            std::map<std::string,
                std::map<std::string, std::string>>>> m_tasks;

    void loadRegionsMapsTasksData(std::string _filename);
    void saveRegionsMapsTasksData(std::string _filename);
    void loadUpgradesData();
};

#endif // DATABASE_H
