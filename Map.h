#ifndef MAP_H
#define MAP_H

#include <map>
#include <string>

class Task;
class Upgrade;

enum class Language;

class Map
{
public:
    Map();
    Map(std::string _code);

    void setCode(std::string _code);
    std::string code();

    void setName(Language _language, std::string _name);
    std::string name(Language _language);

    void addTask(Task* _task);
    std::map<std::string, Task*> tasks();
    Task* task(std::string _taskCode);

    void addUpgrade(Upgrade* _upgrade);
    std::map<std::string, Upgrade*> upgrades();
    Upgrade* upgrade(std::string _upgradeCode);

private:
    std::string m_code;
    std::map<Language, std::string> m_name;
    std::map<std::string, Task*> m_tasks;
    std::map<std::string, Upgrade*> m_upgrades;
};

#endif // MAP_H
