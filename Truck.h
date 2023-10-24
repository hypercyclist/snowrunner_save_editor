#ifndef TRUCK_H
#define TRUCK_H

#include <map>
#include <vector>

enum class Language;

class Truck
{
public:
    Truck();
    Truck(std::string _code);

    void setCode(std::string _code);
    std::string code();

    void setTranslationCode(std::string _code);
    std::string translationCode();

    void setNameCode(std::string _code);
    std::string nameCode();

    void setName(Language _language, std::string _name);
    std::string name(Language _language);

    void addEngineSocketType(std::string _type);
    void addGearboxSocketType(std::string _type);
    void addSuspensionSocketType(std::string _type);
    std::multimap<std::string, std::string> socketTypes();

    void addUpgrade(std::string _upgradeCode);
    std::vector<std::string> upgrades();

private:
    std::string m_code;
    std::string m_translationCode;
    std::string m_nameCode;
    std::map<Language, std::string> m_name;
    std::vector<std::string> m_engineSocketTypes;
    std::vector<std::string> m_gearboxSocketTypes;
    std::vector<std::string> m_suspensionSocketTypes;
    std::vector<std::string> m_upgrades;
};

#endif // TRUCK_H
