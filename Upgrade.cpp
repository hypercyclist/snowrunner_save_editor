#include "Upgrade.h"

Upgrade::Upgrade() :
    m_code(),
    m_middleCode(),
    m_name()
{ }

Upgrade::Upgrade(std::string _code) :
    Upgrade()
{
    m_code = _code;
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
    m_name.insert({_language, _name});
}

std::string Upgrade::name(Language _language)
{
    return m_name[_language];
}
