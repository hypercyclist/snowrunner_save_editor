#include "Task.h"

Task::Task() :
    m_code(),
    m_name()
{ }

Task::Task(std::string _code) :
    Task()
{
    m_code = _code;
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
