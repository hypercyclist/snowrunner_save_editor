#ifndef TASK_H
#define TASK_H

#include <map>

enum class Language;

class Task
{
public:
    Task();
    Task(std::string _code);

    void setRegionCode(std::string _code);
    std::string regionCode();

    void setMapCode(std::string _code);
    std::string mapCode();

    void setCode(std::string _code);
    std::string code();

    void setTranslationCode(std::string _code);
    std::string translationCode();

    void setName(Language _language, std::string _name);
    std::string name(Language _language);

    void setComplete(bool _state);
    bool complete();

private:
    std::string m_regionCode;
    std::string m_mapCode;
    std::string m_code;
    std::string m_translationCode;
    std::map<Language, std::string> m_name;
    bool m_complete;
};

#endif // TASK_H
