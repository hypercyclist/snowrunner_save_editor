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
// Языки жестко запрограммированы.
    m_languageTextNames.insert({Language::RUSSIAN, "russian"});
    m_languageTextNames.insert({Language::CHINESE_TRADITIONAL, "chinese_traditional"});
    m_languageTextNames.insert({Language::CHINESE_SIMPLIFIED, "chinese_simple"});
    m_languageTextNames.insert({Language::BRAZILIAN_PORTUGUESE, "brazilian_portuguese"});
    m_languageTextNames.insert({Language::KOREAN, "korean"});
    m_languageTextNames.insert({Language::ENGLISH, "english"});
    m_languageTextNames.insert({Language::JAPANESE, "japanese"});
    m_languageTextNames.insert({Language::GERMAN, "german"});
    m_languageTextNames.insert({Language::SPANISH, "spanish"});
    m_languageTextNames.insert({Language::FRENCH, "french"});
    m_languageTextNames.insert({Language::CZECH, "czech"});
    m_languageTextNames.insert({Language::ITALIAN, "italian"});
    m_languageTextNames.insert({Language::POLISH, "polish"});
}

// https://stackoverflow.com/questions/50696864/reading-utf-16-file-in-c
void Localization::createLocalizations()
{
    std::string error;

// Найдем и считаем все файлы локализации.
    for (const auto& languagePair : m_languageTextNames)
    {
        std::string filePath = QDir::currentPath().toStdString()
            + "/database/generator_materials/strings/" + languagePair.second + ".str";

        std::ifstream readStream(filePath, std::ios::binary);

// Если файл открыть не удалось, то словарь будет сгенерирован неполным.
// Желательно, если по итогу конкретный словарь имеет 0 загруженных слов
// пресекать переключение программы на этот язык.
        if (!readStream.is_open())
        {
            if (error.size() == 0)
            {
                error += "Warning:\n";
            }
            error += languagePair.second;
            error += " localization will not be created because file:\n";
            error += filePath + " not found.\n";
            continue;
        }

// Я скопировал код чтения UTF16LE со stackoverflow.
// Я понял, что пропускается пару байт BOM. Далее читается текст, где все
// символы кодируются 2 байтами. Предстоит узнать как китайский символ
// помещается в 2 байтах.
        readStream.seekg(0, std::ios::end);
        size_t size = (size_t)readStream.tellg();

        readStream.seekg(2, std::ios::beg);
        size -= 2;

        std::u16string u16((size / 2) + 1, '\0');
        readStream.read((char*)&u16[0], size);

        std::string localizationText = std::wstring_convert<
            std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(u16);

        std::map<std::string, std::string> localizationMap;

// Разбиваем файл на строки. Каждая строка это код + расшифровка.
// Здесь он также фильтруется от лишних пробелов, табов, кавычек.
// А также локализационные коды меняют регистр на нижний.
        std::vector<std::string> lines = Utils::split(localizationText, '\n');
        for (std::string line : lines)
        {
            line = Utils::simplified(line);
            std::string::size_type spaceIndex = line.find(' ');
            std::string code = Utils::stolower(line.substr(0, spaceIndex));
            std::string value = line.substr(spaceIndex + 1, line.length()
                - spaceIndex + 1);
            value = Utils::replace(value, "\"", "");
            localizationMap.insert({code, value});
        }
// Вставляем формализованную локализацию в общий map языков.
        m_localizations.insert({languagePair.first, localizationMap});
    }

//
    loadAppLocalization();
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

void Localization::loadAppLocalization()
{
    std::vector<std::string> appLocalizationCodes = {
//        "UI_SETTINGS_LANGUAGE", // "ЯЗЫК"
        "UI_INGAME_MENU", // "Меню"
        "UI_POLYGON_PACKER_DIALOG_OPEN", // "Открыть"
        "UI_LOG_SAVE_IN_PROGRESS", // "СОХРАНЕНИЕ"
        "UI_OPEN_PROFILE", // "Профиль игрока"
        "UI_POPUP_DEPLOY_MONEY", // "Ваши деньги"
        "UI_NGP_RANK_10", // "Ранг 10"
        "CODEX_LEVEL_UP_HEADER", // "НАБОР ОПЫТА"
        "UI_PLAYER_PROFILE_TAB_TASKS_NAME", // "ПОРУЧЕНИЯ"
        "UI_PLAYER_PROFILE_TAB_CONTRACTS_NAME", // "КОНТРАКТЫ"
        "UI_MM_SETTINGS_REGION", // "Регион"
        "UI_MINIMAP", // "КАРТА"
        "UI_REFRESH", // "Обновить"
        "UI_MOD_BROWSER_MORE_OPTIONS_DISABLE_ALL", // "Включить все"
        "UI_MOD_BROWSER_MORE_OPTIONS_ENABLE_ALL", // "Отключить все"
        "UI_SELECT", // "Выбрать"
        "UI_GARAGE_MODIFICATIONS_UPGRADES", // "Улучшения"
        "UI_PLAYER_PROFILE_STATISTICS_HEADER", // "Статистика"
        "UI_DLC_STORE_ALL_DLCS", // "Все"
        "UI_HUD_NAV_PANEL_GET_UPGRADE", // "Улучшение"
        "UI_HUD_NAV_PANEL_SHOW_TASK", // "Поручение"
        "UI_HUD_EVENT_DISCOVERED_OBJECTIVE_CONTRACT", // "Контракт"
        "UI_TRUCK_PART_ENGINE", // "ДВИГАТЕЛЬ"
        "UI_TRUCK_PART_SUSPENSION",	// "ПОДВЕСКА"
        "UI_TRUCK_PART_GEARBOX", // "КОРОБКА ПЕРЕДАЧ"
        "UI_TRUCK_INFO_DIFF_LOCK", // "ДИФФЕРЕНЦИАЛ"
        "UI_UPGRADE_TRANSFERBOX_AWDSWITCHABLE_NAME", // "Подключаемый полный привод"
        "UI_POLYGON_INFO", // "Информация"
        "UI_SETTINGS_GAME", // "ИГРА"
        "UI_HARDMODE", // "СЛОЖНЫЙ РЕЖИМ"
        "UI_MAIN_MENU_NEW_GAME_LIST_USUAL", // "ОБЫЧНЫЙ РЕЖИМ"
        "UI_NGPLUS", // "Новая Игра +"
        "NEW_GAME_PLUS_HEADER", // "Правила Новой Игры +"
    };
    for (std::string code : appLocalizationCodes)
    {
        for (const auto& languagePair : m_languageTextNames)
        {
            getLocalization(code, languagePair.first);
        }
    }
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

Language Localization::languageByTextName(std::string _languageCode)
{
    for (const auto& languagePair : m_languageTextNames)
    {
        if (languagePair.second == _languageCode)
        {
            return languagePair.first;
        }
    }
    return Language::RUSSIAN;
}

std::map<Language, std::string>& Localization::languageTextNames()
{
    return m_languageTextNames;
}
