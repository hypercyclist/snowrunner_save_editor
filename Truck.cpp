#include "Truck.h"

Truck::Truck() :
    m_code(),
    m_nameCode(),
    m_name(),
    m_upgrades()
{ }

Truck::Truck(std::string _code)
{
    m_code = _code;
}

void Truck::setCode(std::string _code)
{
    m_code = _code;
}

std::string Truck::code()
{
    return m_code;
}

void Truck::setTranslationCode(std::string _code)
{
    m_translationCode = _code;
}

std::string Truck::translationCode()
{
    return m_translationCode;
}

void Truck::setNameCode(std::string _code)
{
    m_nameCode = _code;
}

std::string Truck::nameCode()
{
    return m_nameCode;
}

void Truck::setName(Language _language, std::string _name)
{
    m_name[_language] = _name;
}

std::string Truck::name(Language _language)
{
    return m_name[_language];
}

void Truck::addEngineSocketType(std::string _type)
{
    m_engineSocketTypes.push_back(_type);
}

void Truck::addGearboxSocketType(std::string _type)
{
    m_gearboxSocketTypes.push_back(_type);
}

void Truck::addSuspensionSocketType(std::string _type)
{
    m_suspensionSocketTypes.push_back(_type);
}

std::multimap<std::string, std::string> Truck::socketTypes()
{
    std::multimap<std::string, std::string> result;
    for (std::string& type : m_engineSocketTypes)
        result.insert({"engines", type});
    for (std::string& type : m_gearboxSocketTypes)
        result.insert({"gearboxes", type});
    for (std::string& type : m_gearboxSocketTypes)
        result.insert({"suspensions", type});
    return result;
}

void Truck::addUpgrade(std::string _upgradeCode)
{
    m_upgrades.push_back(_upgradeCode);
}

std::vector<std::string> Truck::upgrades()
{
    return m_upgrades;
}
