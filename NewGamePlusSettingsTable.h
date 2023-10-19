#ifndef NEWGAMEPLUSSETTINGSTABLE_H
#define NEWGAMEPLUSSETTINGSTABLE_H

#include <QWidget>

class Localization;
class NewGamePlusSettings;

namespace Ui { class NewGamePlusSettingsTable; }

class NewGamePlusSettingsTable : public QWidget
{
    Q_OBJECT

public:
    explicit NewGamePlusSettingsTable(QWidget *parent = nullptr);
    ~NewGamePlusSettingsTable();

    void setLocalization(Localization* _localization);
    void setNewGamePlusSettings(NewGamePlusSettings* _newGamePlusSettings);

    void updateTable();

private:
    Ui::NewGamePlusSettingsTable *ui;
    Localization* m_localization;
    NewGamePlusSettings* m_newGamePlusSettings;
};

#endif // NEWGAMEPLUSSETTINGSTABLE_H
