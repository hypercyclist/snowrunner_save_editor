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

    std::map<Language, std::map<std::string, std::string>>& getLocalization();
    std::map<std::string, std::string>& getLocalization(Language _language);
    std::map<Language, std::map<std::string, std::string>>& getLocalizationCache();
    std::map<std::string, std::string>& getLocalizationCache(Language _language);

    std::string languageTextName(Language _language);

private:
    std::map<Language, std::map<std::string, std::string>> m_localizations;
    std::map<Language, std::map<std::string, std::string>> m_localizationsCache;
    Language m_defaultLanguage;
    std::map<Language, std::string> m_languageTextNames;
};

#endif // LOCALIZATION_H
