#include "Localization.h"
#include <QFile>
#include <QTextStream>
#include <QDir>

Localization::Localization() :
    m_localizations(),
    m_defaultLanguage(Language::RUSSIAN)
{ }

void Localization::loadLocalizations()
{
    QFile russianLocalizationFile(QDir::currentPath() + "/localizations/russian.str");
    russianLocalizationFile.open(QIODevice::ReadOnly | QIODevice::Text);

    QMap<QString, QString> russianLocalizationMap;

    QTextStream russianLocalizationStream(&russianLocalizationFile);
    while(!russianLocalizationStream.atEnd())
    {
        QString line = russianLocalizationStream.readLine().simplified();
        QString code = line.left(line.indexOf(' ')).toLower();
        QString value = line.right(line.size()
            - (line.indexOf(' ') + 1)).remove(0, 1).remove(-1, 1);
        russianLocalizationMap.insert(code, value);
    }
    russianLocalizationFile.close();

    m_localizations.insert(Language::RUSSIAN, russianLocalizationMap);
}

void Localization::setDefaultLanguage(Language _language)
{
    m_defaultLanguage = _language;
}

QString Localization::getLocalization(QString _code, Language _language)
{
    _language = _language == Language::DEFAULT ? m_defaultLanguage : _language;
    QString result = m_localizations.value(_language).value(_code.toLower(),
        "No translation");
    return result;
}
