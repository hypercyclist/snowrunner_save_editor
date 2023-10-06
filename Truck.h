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

    void setName(Language _language, std::string _name);
    std::string name(Language _language);

    void addUpgrade(std::string _upgradeCode);
    std::vector<std::string> upgrades();

private:
    std::string m_code;
    std::map<Language, std::string> m_name;
    std::vector<std::string> m_upgrades;
};

#endif // TRUCK_H
