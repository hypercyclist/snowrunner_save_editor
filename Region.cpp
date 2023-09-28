#include "Region.h"
#include "Map.h"

Region::Region() :
    m_code(),
    m_name(),
    m_maps()
{ }

Region::Region(std::string _code) :
    Region()
{
    m_code = _code;
}

void Region::setCode(std::string _code)
{
    m_code = _code;
}

std::string Region::code()
{
    return m_code;
}

void Region::setName(Language _language, std::string _name)
{
    m_name.insert({_language, _name});
}

std::string Region::name(Language _language)
{
    return m_name[_language];
}

void Region::addMap(Map _map)
{
    m_maps.insert({_map.code(), _map});
}

std::map<std::string, Map>* Region::maps()
{
    return &m_maps;
}

Map* Region::map(std::string _mapCode)
{
    return m_maps.find(_mapCode) != m_maps.end() ? &m_maps[_mapCode] : nullptr;
}
