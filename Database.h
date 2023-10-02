#ifndef DATABASE_H
#define DATABASE_H

#include <string>

class Localization;
class GameAtlas;

class Database
{
public:
    Database();
    void createDatabase(std::string _databasePath, std::string _tasksPath,
        std::string _initialCacheBlockPath, std::string _savePath);
    bool loadDatabase(std::string _filename);

    Localization* localization();
    GameAtlas* gameAtlas();

private:
    Localization* m_localization;
    GameAtlas* m_gameAtlas;
};

#endif // DATABASE_H
