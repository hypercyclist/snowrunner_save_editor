#ifndef DATABASE_H
#define DATABASE_H

#include <string>

class Localization;
class GameAtlas;

class Database
{
public:
    Database();
    void createDatabase(std::string _databasePath);
    bool loadDatabase(std::string _databasePath);

    Localization* localization();
    GameAtlas* gameAtlas();

private:
    Localization* m_localization;
    GameAtlas* m_gameAtlas;
};

#endif // DATABASE_H
