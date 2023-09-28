#ifndef TASK_H
#define TASK_H

#include <map>

class Task
{
public:
    Task();
    Task(std::string _code);

    void setCode(std::string _code);
    std::string code();
private:
    std::string m_code;
};

#endif // TASK_H
