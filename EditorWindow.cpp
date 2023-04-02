#include "EditorWindow.h"
#include "ui_EditorWindow.h"

#include <QCheckBox>
#include <QDebug>
#include <QFileDialog>
#include <QAbstractItemModel>
#include "rapidjson/writer.h"

EditorWindow::EditorWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::EditorWindow),
    text("")
{
    ui->setupUi(this);
    loadLocalizations();

    QFile tasksFile("D:\\product\\programming\\in_progress\\snowrunner_save_editor\\"
        "build-snowrunner_save_editor-Desktop_Qt_5_12_12_MinGW_64_bit-Debug\\debug\\tasks.json");
    tasksFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString tasksText = tasksFile.readAll();
    tasksFile.close();
    rapidjson::Document tasksjsonDocument;
    tasksjsonDocument.Parse(tasksText.toUtf8());

    ui->completeTasksTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QAbstractItemModel* tasksTableModel = ui->completeTasksTable->model();
    rapidjson::Value& tasksArray = tasksjsonDocument["Tasks"].GetArray();
    QSet<QString> regions;
    QSet<QString> maps;

    for (rapidjson::SizeType i = 0; i < tasksArray.Size(); i++)
    {
        ui->completeTasksTable->insertRow(i);
        QString taskCode = tasksArray[i]["task_code"].GetString();

        QString region = getLocalization(taskCode.left(5), Language::RUSSIAN);
        tasksTableModel->setData(tasksTableModel->index(i, 0), region);

        QString map = getLocalization(taskCode.left(8) + "_NAME", Language::RUSSIAN);
        if (map == "No translation")
            map = getLocalization(taskCode.left(8) + "_NEW_NAME", Language::RUSSIAN);
        if (map == "No translation")
            map = getLocalization("LEVEL_" + taskCode.left(8) + "_NAME", Language::RUSSIAN);
        if (map == "No translation")
            map = getLocalization("LEVEL_" + taskCode.left(8), Language::RUSSIAN);
        tasksTableModel->setData(tasksTableModel->index(i, 1), map);

        QString taskName = getLocalization(taskCode, Language::RUSSIAN);
        tasksTableModel->setData(tasksTableModel->index(i, 2), taskName);

        QCheckBox* statusCheckBox = new QCheckBox();
        QWidget* statusContainer = new QWidget();
        QHBoxLayout* statusContainerLayout = new QHBoxLayout();
        statusContainerLayout->setContentsMargins(0, 0, 0, 0);
        statusContainerLayout->setAlignment( Qt::AlignCenter );
        statusContainerLayout->addWidget(statusCheckBox);
        statusContainer->setLayout(statusContainerLayout);
        ui->completeTasksTable->setCellWidget(i, 3, statusContainer);

        regions.insert(region);
        maps.insert(map);
    }
    for (QString _region : regions)
    {
        ui->regionFilterCombobox->addItem(_region);
    }
    for (QString _map : maps)
    {
        ui->mapFilterCombobox->addItem(_map);
    }
}

EditorWindow::~EditorWindow()
{
    delete ui;
}

void EditorWindow::on_menuOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "C:\\Users\\EV7\\Desktop\\", tr("CFG (*.cfg)"));
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    text = file.readAll();
    file.close();
    jsonDocument.Parse(text.toUtf8());

    rapidjson::Value& persistentProfileData =
        jsonDocument["CompleteSave"]["SslValue"]["persistentProfileData"];

    ui->moneyCountSpinBox->setValue(persistentProfileData["money"].GetInt());
    ui->rankSpinBox->setValue(persistentProfileData["rank"].GetInt());
    ui->experienceCountSpinBox->setValue(persistentProfileData["experience"].GetInt());

    connect(ui->moneyCountSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int _value) { });
    connect(ui->rankSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int _value) { });
    connect(ui->experienceCountSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int _value) { });
}

void EditorWindow::on_menuSave_triggered()
{
//    rapidjson::GenericObject& persistentProfileData =
//        jsonDocument["CompleteSave"]["SslValue"]["persistentProfileData"].GetObject();
    qDebug() << "test";
    jsonDocument["CompleteSave"]["SslValue"]["persistentProfileData"]["money"] = ui->moneyCountSpinBox->value();
    jsonDocument["CompleteSave"]["SslValue"]["persistentProfileData"]["rank"] = ui->rankSpinBox->value();
    jsonDocument["CompleteSave"]["SslValue"]["persistentProfileData"]["experience"] = ui->experienceCountSpinBox->value();

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    jsonDocument.Accept(writer);

    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"), "", tr("CFG (*.cfg)"));
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(buffer.GetString());
    file.close();
}

void EditorWindow::loadLocalizations()
{
    QFile russianLocalizationFile("D:\\product\\programming\\in_progress\\snowrunner_save_editor\\"
        "build-snowrunner_save_editor-Desktop_Qt_5_12_12_MinGW_64_bit-Debug\\debug\\russian.str");
    russianLocalizationFile.open(QIODevice::ReadOnly | QIODevice::Text);

    QMap<QString, QString> russianLocalizationMap;

    QTextStream russianLocalizationStream(&russianLocalizationFile);
    while(!russianLocalizationStream.atEnd())
    {
        QString line = russianLocalizationStream.readLine().simplified();
        QString code = line.left(line.indexOf(' ')).toLower();
        QString value = line.right(line.size() - (line.indexOf(' ') + 1)).remove(0, 1).remove(-1, 1);
        russianLocalizationMap.insert(code, value);
    }
    russianLocalizationFile.close();

    m_localizations.insert(Language::RUSSIAN, russianLocalizationMap);
}

QString EditorWindow::getLocalization(QString _code, Language _language)
{
    QString result = m_localizations.value(_language).value(_code.toLower(), "No translation");
    return result;
}



















