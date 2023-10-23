#include "Task.h"

Task::Task() :
    m_code(),
    m_name(),
    m_complete(false)
{ }

Task::Task(std::string _code) :
    Task()
{
    m_code = _code;
}

void Task::setRegionCode(std::string _code)
{
    m_regionCode = _code;
}

std::string Task::regionCode()
{
    return m_regionCode;
}

void Task::setMapCode(std::string _code)
{
    m_mapCode = _code;
}

std::string Task::mapCode()
{
    return m_mapCode;
}

void Task::setCode(std::string _code)
{
    m_code = _code;
}

std::string Task::code()
{
    return m_code;
}

void Task::setName(Language _language, std::string _name)
{
    m_name[_language] = _name;
}

std::string Task::name(Language _language)
{
    return m_name[_language];
}

void Task::setComplete(bool _state)
{
    m_complete = _state;
}

bool Task::complete()
{
    return m_complete;
}
