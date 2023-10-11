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

void Truck::addUpgrade(std::string _upgradeCode)
{
    m_upgrades.push_back(_upgradeCode);
}

std::vector<std::string> Truck::upgrades()
{
    return m_upgrades;
}
