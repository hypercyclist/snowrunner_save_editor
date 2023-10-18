#ifndef SAVEFILE_H
#define SAVEFILE_H

#include "rapidjson/document.h"

#include <string>
#include <vector>

class SaveFile
{
public:
    SaveFile();
    SaveFile(std::string _fullFileName);
    ~SaveFile();

    bool loadSaveFile(std::string _fullFileName);
    bool loaded();
    bool saveSaveFile(std::string _fullFileName);

    std::string error();

    int money();
    void setMoney(int _value);

    int rank();
    void setRank(int _value);

    int experience();
    void setExperience(int _value);

    int gameDifficultyMode();
    void setGameDifficultyMode(int _value);

    int isHardMode();
    void setIsHardMode(int _value);

    std::vector<std::string> finishedObjs();
    void setFinishedObjs(std::vector<std::string> _value);

    std::vector<std::string> foundedUpgrades();
    void setFoundedUpgrades(std::vector<std::string> _value);

    std::vector<std::string> receivedUpgrades();
    void setReceivedUpgrades(std::vector<std::string> _value);

protected:
    bool m_loaded;
    std::string m_error;
    rapidjson::Document saveFileJsonDocument;
    std::string completeSaveJMN;

    int m_money;
    int m_rank;
    int m_experience;
    int m_gameDifficultyMode;
    bool m_isHardMode;
    std::vector<std::string> m_finishedObjs;
    std::vector<std::string> m_foundedUpgrades;
    std::vector<std::string> m_receivedUpgrades;
};

#endif // SAVEFILE_H
