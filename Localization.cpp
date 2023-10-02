#include "Localization.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <iostream>
#include "Utils.h"
#include <fstream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>

Localization::Localization() :
    m_localizations(),
    m_localizationsCache(),
    m_localizationsTemplates(),
    m_defaultLanguage(Language::RUSSIAN),
    m_languageTextNames()
{
    m_languageTextNames.insert({Language::RUSSIAN, "russian"});
    m_languageTextNames.insert({Language::RUSSIAN, "english"});
}

// https://stackoverflow.com/questions/50696864/reading-utf-16-file-in-c
void Localization::createLocalizations()
{
    std::string fileName = QDir::currentPath().toStdString()
        + "/database/generator_materials/russian.str";

    std::ifstream readStream(fileName, std::ios::binary);
    readStream.seekg(0, std::ios::end);
    size_t size = (size_t)readStream.tellg();

    readStream.seekg(2, std::ios::beg);
    size -= 2;

    std::u16string u16((size / 2) + 1, '\0');
    readStream.read((char*)&u16[0], size);

    std::string localizationText = std::wstring_convert<
        std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(u16);


    std::map<std::string, std::string> russianLocalizationMap;

    std::vector<std::string> lines = Utils::split(localizationText, '\n');
    for (std::string line : lines)
    {
        line = Utils::simplified(line);
        std::string::size_type spaceIndex = line.find(' ');
        std::string code = Utils::stolower(line.substr(0, spaceIndex));
        std::string value = line.substr(spaceIndex + 1, line.length() - spaceIndex + 1);
        value = Utils::replace(value, "\"", "");
        russianLocalizationMap.insert({code, value});
    }

    m_localizations.insert(std::pair<Language,
        std::map<std::string, std::string>>(Language::RUSSIAN,
            russianLocalizationMap));
}

bool Localization::loadLocalizations(std::string _filename)
{
    m_localizations.clear();
    m_localizationsCache.clear();

    std::ifstream readStream(_filename);

    if (!readStream.is_open()) {
        return false;
    }

    std::stringstream readBuffer;
    readBuffer << readStream.rdbuf();
    readStream.close();
    std::string databaseText = readBuffer.str();

    rapidjson::Document databaseJsonDocument;
    databaseJsonDocument.Parse(databaseText.c_str());

    if (!databaseJsonDocument.IsObject()) {
        return false;
    }

    auto allocator = databaseJsonDocument.GetAllocator();

    if (!databaseJsonDocument.HasMember("Localization")) {
        return false;
    }

    rapidjson::Value& localizationObject = databaseJsonDocument["Localization"];
    for (const auto& languagePair : m_languageTextNames)
    {
        if (!localizationObject.HasMember(languagePair.second.c_str())) {
            continue;
        }
        rapidjson::Value& languageObject = localizationObject[languagePair.second.c_str()];
        for (auto languageIt = languageObject.MemberBegin(); languageIt != languageObject.MemberEnd(); languageIt++)
        {
            m_localizations[languagePair.first][languageIt->name.GetString()] = languageIt->value.GetString();
            m_localizationsCache[languagePair.first][languageIt->name.GetString()] = languageIt->value.GetString();
        }
    }
    return true;
}

void Localization::saveLocalizationCache(std::string _filename)
{
    std::ifstream readStream(_filename);
    std::stringstream readBuffer;
    readBuffer << readStream.rdbuf();
    readStream.close();
    std::string databaseText = readBuffer.str();

    rapidjson::Document databaseJsonDocument;
    databaseJsonDocument.Parse(databaseText.c_str());
    auto allocator = databaseJsonDocument.GetAllocator();

    rapidjson::Value localizationObject;
    if (databaseJsonDocument.HasMember("Localization"))
    {
        localizationObject = databaseJsonDocument["Localization"];
    }
    else
    {
        localizationObject.SetObject();
    }


    for (const auto& languagePair : m_localizationsCache)
    {
        std::string languageCode = m_languageTextNames[languagePair.first];

        rapidjson::Value languageObject;
        if (localizationObject.HasMember(languageCode.c_str()))
        {
            languageObject = localizationObject[languageCode.c_str()];
        }
        else
        {
            languageObject.SetObject();
        }

        rapidjson::Value languageCodeValue;
        languageCodeValue.SetString(languageCode.c_str(), languageCode.length(), allocator);

        for (const auto& wordPair : languagePair.second)
        {
            if (languageObject.HasMember(wordPair.first.c_str())) {
                continue;
            }

            rapidjson::Value wordString;
            wordString.SetString(wordPair.second.c_str(), wordPair.second.length(), allocator);

            rapidjson::Value wordCode;
            wordCode.SetString(wordPair.first.c_str(), wordPair.first.length(), allocator);

            languageObject.AddMember(wordCode, wordString, allocator);
        }

        for (const auto& wordPair : m_localizationsTemplates[languagePair.first])
        {
            if (languageObject.HasMember(wordPair.first.c_str())) {
                continue;
            }

            rapidjson::Value wordString;
            wordString.SetString(wordPair.second.c_str(), wordPair.second.length(), allocator);

            rapidjson::Value wordCode;
            wordCode.SetString(wordPair.first.c_str(), wordPair.first.length(), allocator);

            languageObject.AddMember(wordCode, wordString, allocator);
        }

        if (localizationObject.HasMember(languageCode.c_str()))
        {
            localizationObject[languageCode.c_str()] = languageObject;
        }
        else
        {
            localizationObject.AddMember(languageCodeValue, languageObject, allocator);
        }
    }

    if (databaseJsonDocument.HasMember("Localization"))
    {
        databaseJsonDocument["Localization"] = localizationObject;
    }
    else
    {
        databaseJsonDocument.AddMember("Localization", localizationObject, allocator);
    }


    rapidjson::StringBuffer writeBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(writeBuffer);
    databaseJsonDocument.Accept(writer);

    std::ofstream writeStream(_filename);
    writeStream << writeBuffer.GetString();
    writeStream.close();
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
        m_localizationsCache[_language] = std::map<std::string, std::string>();
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
            m_localizationsCache[_language][Utils::stolower(_code)] = result;
        }
    }
    return result;
}

Language Localization::defaultLanguage()
{
    return m_defaultLanguage;
}

void Localization::addLocalizationsTemplate(std::string _code, std::string _value)
{
    for (const auto& languagePair : m_languageTextNames)
    {
        m_localizationsTemplates[languagePair.first][_code] = _value;
    }
}

std::map<Language, std::map<std::string, std::string>>& Localization::getLocalization()
{
    return m_localizations;
}

std::map<std::string, std::string>& Localization::getLocalization(Language _language)
{
    return m_localizations[_language];
}

std::map<Language, std::map<std::string, std::string>>& Localization::getLocalizationCache()
{
    return m_localizationsCache;
}

std::map<std::string, std::string>& Localization::getLocalizationCache(Language _language)
{
    return m_localizationsCache[_language];
}

std::map<Language, std::map<std::string, std::string>>& Localization::getLocalizationTemplates()
{
    return m_localizationsTemplates;
}

std::string Localization::languageTextName(Language _language)
{
    return m_languageTextNames[_language];
}

std::map<Language, std::string>& Localization::languageTextNames()
{
    return m_languageTextNames;
}
