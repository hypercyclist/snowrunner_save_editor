#ifndef DATABASE_H
#define DATABASE_H

#include <string>

class Localization;
class GameAtlas;
class NewGamePlusSettings;

class Database
{
public:
    Database();
    void createDatabase(std::string _databasePath);
    void createDatabase2(std::string _databasePath);
    bool loadDatabase(std::string _databasePath);

    Localization* localization();
    GameAtlas* gameAtlas();
    NewGamePlusSettings* newGamePlusSettings();

private:
    Localization* m_localization;
    GameAtlas* m_gameAtlas;
    NewGamePlusSettings* m_newGamePlusSettings;
};

#endif // DATABASE_H
