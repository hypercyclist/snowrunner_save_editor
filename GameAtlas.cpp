#include "GameAtlas.h"
#include "Region.h"

GameAtlas::GameAtlas() :
    m_regions()
{ }

void GameAtlas::addRegion(Region* _region)
{
    m_regions.insert({_region->code(), _region});
}

std::map<std::string, Region*>* GameAtlas::regions()
{
    return &m_regions;
}

Region* GameAtlas::region(std::string _regionCode)
{
    return m_regions.find(_regionCode) != m_regions.end() ?
        m_regions[_regionCode] : nullptr;
}
