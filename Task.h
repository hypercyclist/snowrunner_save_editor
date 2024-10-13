#ifndef TASK_H
#define TASK_H

#include <map>
#include <string>

enum class Language;

class Task
{
public:
    Task();
    Task(std::string _code);

    void setCode(std::string _code);
    std::string code();

    void setName(Language _language, std::string _name);
    std::string name(Language _language);

    void setComplete(bool _state);
    bool complete();

private:
    std::string m_code;
    std::map<Language, std::string> m_name;
    bool m_complete;
};

#endif // TASK_H
