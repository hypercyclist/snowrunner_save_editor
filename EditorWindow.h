#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include <rapidjson/document.h>

class Database;

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

private:
    Ui::EditorWindow *ui;
    Database* m_database;
    rapidjson::Document jsonDocument;
    QString text;
};
#endif // EDITORWINDOW_H