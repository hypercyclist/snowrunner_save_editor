#ifndef REGION_H
#define REGION_H

#include <string>
#include <map>

class Map;

enum class Language;

class Region
{
public:
    Region();
    Region(std::string _code);

    void setCode(std::string _code);
    std::string code();

    void setTranslationCode(std::string _code);
    std::string translationCode();

    void setName(Language _language, std::string _name);
    std::string name(Language _language);

    void addMap(Map* _map);
    std::map<std::string, Map*> maps();
    Map* map(std::string _mapCode);

private:
    std::string m_code;
    std::string m_translationCode;
    std::map<Language, std::string> m_name;
    std::map<std::string, Map*> m_maps;
};

#endif // REGION_H
