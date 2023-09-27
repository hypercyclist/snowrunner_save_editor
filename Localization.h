#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <string>
#include <map>

enum class Language { RUSSIAN, ENGLISH };

class Localization
{
public:
    Localization();
    void loadLocalizations();
    void setDefaultLanguage(Language _language);
    std::string getLocalization(std::string _code);
    std::string getLocalization(std::string _code, Language _language);
    Language defaultLanguage();
private:
    std::map<Language, std::map<std::string, std::string>> m_localizations;
    std::map<Language, std::map<std::string, std::string>> m_localizationsCache;
    Language m_defaultLanguage;
};

#endif // LOCALIZATION_H
