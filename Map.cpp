#include "Map.h"
#include "Task.h"

Map::Map() :
    m_code(),
    m_tasks()
{ }

Map::Map(std::string _code) :
    Map()
{
    m_code = _code;
}

void Map::setCode(std::string _code)
{
    m_code = _code;
}

std::string Map::code()
{
    return m_code;
}

void Map::addTask(Task _task)
{
    m_tasks.insert({_task.code(), _task});
}

std::map<std::string, Task>* Map::tasks()
{
    return &m_tasks;
}

Task* Map::task(std::string _taskCode)
{
    return m_tasks.find(_taskCode) != m_tasks.end() ?
        &m_tasks[_taskCode] : nullptr;
}
