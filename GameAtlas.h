#ifndef GAMEATLAS_H
#define GAMEATLAS_H

#include <map>

class Region;
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
    void saveLocalizationCache(std::string _filename);

private:
    std::map<std::string, Region*> m_regions;
    Localization* m_localization;
};

#endif // GAMEATLAS_H
