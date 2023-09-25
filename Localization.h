#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <QString>
#include <QMap>

enum class Language { RUSSIAN, ENGLISH, DEFAULT };

class Localization
{
public:
    Localization();
    void loadLocalizations();
    void setDefaultLanguage(Language _language);
    QString getLocalization(QString _code, Language _language = Language::DEFAULT);
private:
    QMap<Language, QMap<QString, QString>> m_localizations;
    Language m_defaultLanguage;
};

#endif // LOCALIZATION_H
