#include "Localization.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <iostream>
#include "Utils.h"

Localization::Localization() :
    m_localizations(),
    m_defaultLanguage(Language::RUSSIAN)
{ }

// Заменить Qt на C++.
void Localization::loadLocalizations()
{
    QFile russianLocalizationFile(QDir::currentPath() + "/database/generator_materials/russian.str");
    russianLocalizationFile.open(QIODevice::ReadOnly | QIODevice::Text);

    std::map<std::string, std::string> russianLocalizationMap;

    QTextStream russianLocalizationStream(&russianLocalizationFile);
    while(!russianLocalizationStream.atEnd())
    {
        QString line = russianLocalizationStream.readLine().simplified();
        QString code = line.left(line.indexOf(' ')).toLower();
        QString value = line.right(line.size()
            - (line.indexOf(' ') + 1)).remove(0, 1).remove(-1, 1);
        russianLocalizationMap.insert(std::pair<std::string, std::string>(
            code.toStdString(), value.toStdString()));
    }
    russianLocalizationFile.close();

    m_localizations.insert(std::pair<Language,
        std::map<std::string, std::string>>(Language::RUSSIAN,
            russianLocalizationMap));
}

void Localization::setDefaultLanguage(Language _language)
{
    m_defaultLanguage = _language;
}

std::string Localization::getLocalization(std::string _code)
{
    return getLocalization(_code, m_defaultLanguage);
}

// Нет обработки отсутствия языка в map.
std::string Localization::getLocalization(std::string _code, Language _language)
{
    if (m_localizationsCache.find(_language) == m_localizationsCache.end())
    {
        m_localizationsCache.insert(std::pair<Language,
            std::map<std::string, std::string>>(
                _language, std::map<std::string, std::string>()));
    }

    auto translationIt = m_localizationsCache[_language].find(Utils::stolower(_code));
    std::string result = translationIt != m_localizationsCache[_language].end() ?
        translationIt->second : "none";

    if (result == "none")
    {
        translationIt = m_localizations[_language].find(Utils::stolower(_code));
        result = translationIt != m_localizations[_language].end() ?
            translationIt->second : "none";

        if (result != "none")
        {
            m_localizationsCache[_language].insert(
                std::pair<std::string, std::string>(Utils::stolower(_code), result));
        }
    }
    return result;
}

Language Localization::defaultLanguage()
{
    return m_defaultLanguage;
}
