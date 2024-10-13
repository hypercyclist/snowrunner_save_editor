#ifndef UPGRADE_H
#define UPGRADE_H

#include <map>
#include <vector>
#include <string>

enum class Language;

enum class UpgradeType { UNKNOWN, ENGINE, SUSPENSION, GEARBOX, DIFFLOCK, TRANSFERBOX };

class Upgrade
{
public:
    Upgrade();
    Upgrade(std::string _code);

    void setType(UpgradeType _type);
    UpgradeType type();

    void setCode(std::string _code);
    std::string code();

    void setMiddleCode(std::string _code);
    std::string middleCode();

    void setName(Language _language, std::string _name);
    std::string name(Language _language);

    void setReceived(bool _state);
    bool received();

    void addTruck(std::string _truckCode);
    std::vector<std::string> trucks();

private:
    std::string m_code;
    UpgradeType m_type;
    std::string m_middleCode;
    std::map<Language, std::string> m_name;
    std::vector<std::string> m_trucks;
    bool m_received;
};

#endif // UPGRADE_H
