#ifndef GAMEATLAS_H
#define GAMEATLAS_H

#include <map>
class Region;

class GameAtlas
{
public:
    GameAtlas();
    void addRegion(Region* _region);
    std::map<std::string, Region*>* regions();
    Region* region(std::string _regionCode);
private:
    std::map<std::string, Region*> m_regions;
};

#endif // GAMEATLAS_H
