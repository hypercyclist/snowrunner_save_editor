#ifndef DATABASE_H
#define DATABASE_H

#include <string>

class Localization;
class GameAtlas;

class Database
{
public:
    Database();
    void createDatabase();
    void loadDatabase();

private:
    Localization* m_localization;
    GameAtlas* m_gameAtlas;

    void loadRegionsMapsTasksData(std::string _filename);
    void saveRegionsMapsTasksData(std::string _filename);
    void saveLocalizationCache(std::string _filename);
    void loadUpgradesData(std::string _saveFileName,
        std::string _initialCacheBlockFileName);
};

#endif // DATABASE_H
