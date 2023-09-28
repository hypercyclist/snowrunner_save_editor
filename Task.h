#ifndef TASK_H
#define TASK_H

#include <map>

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

private:
    std::string m_code;
    std::map<Language, std::string> m_name;
};

#endif // TASK_H
