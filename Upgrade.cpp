#include "Upgrade.h"

Upgrade::Upgrade() :
    m_code(),
    m_type(UpgradeType::UNKNOWN),
    m_middleCode(),
    m_name(),
    m_trucks(),
    m_received(false)
{ }

Upgrade::Upgrade(std::string _code) :
    Upgrade()
{
    m_code = _code;
}

void Upgrade::setType(UpgradeType _type)
{
    m_type = _type;
}

UpgradeType Upgrade::type()
{
    return m_type;
}

void Upgrade::setCode(std::string _code)
{
    m_code = _code;
}

std::string Upgrade::code()
{
    return m_code;
}

void Upgrade::setMiddleCode(std::string _code)
{
    m_middleCode = _code;
}

std::string Upgrade::middleCode()
{
    return m_middleCode;
}

void Upgrade::setName(Language _language, std::string _name)
{
    m_name[_language] = _name;
}

std::string Upgrade::name(Language _language)
{
    return m_name[_language];
}

void Upgrade::setReceived(bool _state)
{
    m_received = _state;
}

bool Upgrade::received()
{
    return m_received;
}

void Upgrade::addTruck(std::string _truckCode)
{
    m_trucks.push_back(_truckCode);
}

std::vector<std::string> Upgrade::trucks()
{
    return m_trucks;
}
