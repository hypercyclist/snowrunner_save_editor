#include "Map.h"
#include "Task.h"
#include "Upgrade.h"

Map::Map() :
    m_code(),
    m_name(),
    m_tasks(),
    m_upgrades()
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

void Map::setName(Language _language, std::string _name)
{
    m_name[_language] = _name;
}

std::string Map::name(Language _language)
{
    return m_name[_language];
}

void Map::addTask(Task* _task)
{
    m_tasks[_task->code()] = _task;
}

std::map<std::string, Task*> Map::tasks()
{
    return m_tasks;
}

Task* Map::task(std::string _taskCode)
{
    return m_tasks.find(_taskCode) != m_tasks.end() ?
        m_tasks[_taskCode] : nullptr;
}

void Map::addUpgrade(Upgrade* _upgrade)
{
    m_upgrades[_upgrade->code()] = _upgrade;
}

std::map<std::string, Upgrade*> Map::upgrades()
{
    return m_upgrades;
}

Upgrade* Map::upgrade(std::string _upgradeCode)
{
    return m_upgrades.find(_upgradeCode) != m_upgrades.end() ?
        m_upgrades[_upgradeCode] : nullptr;
}
