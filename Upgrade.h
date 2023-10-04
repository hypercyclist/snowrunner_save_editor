#ifndef UPGRADE_H
#define UPGRADE_H

#include <map>

enum class Language;

class Upgrade
{
public:
    Upgrade();
    Upgrade(std::string _code);

    void setCode(std::string _code);
    std::string code();

    void setMiddleCode(std::string _code);
    std::string middleCode();

    void setName(Language _language, std::string _name);
    std::string name(Language _language);

    void setReceived(bool _state);
    bool received();

private:
    std::string m_code;
    std::string m_middleCode;
    std::map<Language, std::string> m_name;
    bool m_received;
};

#endif // UPGRADE_H
