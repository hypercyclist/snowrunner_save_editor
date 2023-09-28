#include "Task.h"

Task::Task() :
    m_code()
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
