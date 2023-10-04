#ifndef GAMEATLAS_H
#define GAMEATLAS_H

#include <map>
#include <vector>

class Region;
class Task;
class Upgrade;
class Localization;

class GameAtlas
{
public:
    GameAtlas();

    void setLocalization(Localization* _localization);

    void addRegion(Region* _region);
    std::map<std::string, Region*> regions();
    Region* region(std::string _regionCode);

    void createGameAtlasData(std::string _filename);
    void createUpgradesData(std::string _saveFileName,
        std::string _initialCacheBlockFileName);
    bool loadGameAtlas(std::string _filename);
    void saveGameAtlasData(std::string _filename);

    std::vector<Task*> completedTasks();
    void setTasksCompleteFromVectorCodes(std::vector<std::string> _codes);
    std::vector<Upgrade*> receivedUpgrades();
    void setUpgradesReceivedFromVectorCodes(std::vector<std::string> _codes);

private:
    std::map<std::string, Region*> m_regions;
    Localization* m_localization;
};

#endif // GAMEATLAS_H
