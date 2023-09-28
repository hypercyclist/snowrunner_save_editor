#ifndef MAP_H
#define MAP_H

#include <map>

class Task;

class Map
{
public:
    Map();
    Map(std::string _code);

    void setCode(std::string _code);
    std::string code();

    void addTask(Task _task);
    std::map<std::string, Task>* tasks();
    Task* task(std::string _taskCode);
private:
    std::string m_code;
    std::map<std::string, Task> m_tasks;
};

#endif // MAP_H
