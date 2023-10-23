#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <string>
#include <map>

enum class Language {
    RUSSIAN,
    CHINESE_TRADITIONAL,
    CHINESE_SIMPLIFIED,
    BRAZILIAN_PORTUGUESE,
    KOREAN,
    ENGLISH,
    JAPANESE,
    GERMAN,
    SPANISH,
    FRENCH,
    CZECH,
    ITALIAN,
    POLISH
};

class Localization
{
public:
    Localization();
    void createLocalizations();
    bool loadLocalizations(std::string _filename);
    void loadAppLocalization();
    void saveLocalizationCache(std::string _filename);
    void setDefaultLanguage(Language _language);
    std::string localization(std::string _code);
    std::string localization(std::string _code, Language _language);
    Language defaultLanguage();
    void addLocalizationsTemplate(std::string _code, std::string _value);

    std::map<Language, std::map<std::string, std::string>>& localization();
    std::map<std::string, std::string>& localization(Language _language);
    std::map<Language, std::map<std::string, std::string>>& localizationCache();
    std::map<std::string, std::string>& localizationCache(Language _language);
    std::map<Language, std::map<std::string, std::string>>& localizationTemplates();

    std::string languageTextName(Language _language);
    Language languageByTextName(std::string _languageCode);
    std::map<Language, std::string>& languageTextNames();

private:
    std::map<Language, std::map<std::string, std::string>> m_localizations;
    std::map<Language, std::map<std::string, std::string>> m_localizationsCache;
    std::map<Language, std::map<std::string, std::string>> m_localizationsTemplates;
    Language m_defaultLanguage;
    std::map<Language, std::string> m_languageTextNames;
};

#endif // LOCALIZATION_H
