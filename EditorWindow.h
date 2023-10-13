#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>

class AppConfig;
class Database;
class SaveFile;
enum class Language;

namespace Ui { class EditorWindow; }

class EditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    EditorWindow(QWidget *parent = nullptr);
    ~EditorWindow();

private slots:
    void on_menuOpen_triggered();
    void on_menuSave_triggered();
    void applyLanguage(Language _language);

    void on_menuInformation_triggered();

private:
    Ui::EditorWindow *ui;
    AppConfig* m_appConfig;
    Database* m_database;
    SaveFile* m_saveFile;
    QString text;

    void setTabsEnabled(bool _state);
    void configureTasksTable();
    void configureUpgradesTable();
    void connectLanguageButtonsToFunctions();
};
#endif // EDITORWINDOW_H
