#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include <QMap>
#include "rapidjson/document.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EditorWindow; }
QT_END_NAMESPACE

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
    rapidjson::Document jsonDocument;
    QString text;

    enum class Language { RUSSIAN, ENGLISH };
    QMap<Language, QMap<QString, QString>> m_localizations;
    void loadLocalizations();
    QString getLocalization(QString _code, Language _language);
};
#endif // EDITORWINDOW_H
