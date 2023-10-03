#include "EditorWindow.h"
#include "ui_EditorWindow.h"

#include <QCheckBox>
#include <QDebug>
#include <QFileDialog>
#include <QAbstractItemModel>
#include "Localization.h"
#include "Database.h"
#include <rapidjson/writer.h>
#include "GameAtlas.h"
#include "Task.h"

#include <fstream>
#include <iostream>

EditorWindow::EditorWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::EditorWindow),
    m_database(nullptr),
    text("")
{
    ui->setupUi(this);

    std::string currentPath = QDir::currentPath().toStdString();
    std::string databasePath = currentPath + "/database/database.json";
    std::string tasksPath = currentPath + "/database/generator_materials/tasks.json";
    std::string initialCacheBlockPath = currentPath + "/database/generator_materials/initial.cache_block";
    std::string savePath = currentPath + "/database/generator_materials/CompleteSave.cfg";

    m_database = new Database();
    bool databaseLoaded = m_database->loadDatabase(databasePath);
    if (!databaseLoaded)
    {
        m_database->createDatabase(databasePath, tasksPath,
            initialCacheBlockPath, savePath);
    }

    ui->completeTasksTable->setGameAtlas(m_database->gameAtlas());
    ui->completeTasksTable->setFilterBarWidget(ui->completeTasksTableBar);
    ui->completeTasksTableBar->setGameAtlas(m_database->gameAtlas());
    ui->completeTasksTableBar->filterMaps();
    ui->completeTasksTable->updateTasksTable();

    ui->upgradesTable->setGameAtlas(m_database->gameAtlas());
    ui->upgradesTable->setFilterBarWidget(ui->completeTasksTableBar);
    ui->upgradesTableBar->setGameAtlas(m_database->gameAtlas());
    ui->upgradesTableBar->filterMaps();
    ui->upgradesTable->updateTasksTable();
}

EditorWindow::~EditorWindow()
{
    delete ui;
}

void EditorWindow::on_menuOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "", tr("CFG (*.cfg)"));
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    text = file.readAll();
    file.close();
    jsonDocument.Parse(text.toUtf8());

    rapidjson::Value& sslValue = jsonDocument["CompleteSave"]["SslValue"];
    rapidjson::Value& persistentProfileData = sslValue["persistentProfileData"];

    ui->moneyCountSpinBox->setValue(persistentProfileData["money"].GetInt());
    ui->rankSpinBox->setValue(persistentProfileData["rank"].GetInt());
    ui->experienceCountSpinBox->setValue(persistentProfileData["experience"].GetInt());

    std::vector<std::string> finishedObjs;
    rapidjson::Value& finishedObjsArray = sslValue["finishedObjs"].GetArray();
    for (unsigned int i = 0; i < finishedObjsArray.Size(); i++)
    {
        finishedObjs.push_back(finishedObjsArray[i].GetString());
    }
    m_database->gameAtlas()->setTasksCompleteFromVectorCodes(finishedObjs);
    ui->completeTasksTable->updateTasksTable();
}

void EditorWindow::on_menuSave_triggered()
{
    jsonDocument["CompleteSave"]["SslValue"]["persistentProfileData"]["money"] = ui->moneyCountSpinBox->value();
    jsonDocument["CompleteSave"]["SslValue"]["persistentProfileData"]["rank"] = ui->rankSpinBox->value();
    jsonDocument["CompleteSave"]["SslValue"]["persistentProfileData"]["experience"] = ui->experienceCountSpinBox->value();
    rapidjson::Value finishedObjsArray;
    finishedObjsArray.SetArray();
    for (Task* task : m_database->gameAtlas()->completedTasks())
    {
        rapidjson::Value strVal;
        strVal.SetString(task->code().c_str(), task->code().length(), jsonDocument.GetAllocator());
        finishedObjsArray.PushBack(strVal, jsonDocument.GetAllocator());
    }
    jsonDocument["CompleteSave"]["SslValue"]["finishedObjs"] = finishedObjsArray;
    jsonDocument["CompleteSave"]["SslValue"]["discoveredObjectives"] = finishedObjsArray;

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    jsonDocument.Accept(writer);

    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"), "", tr("CFG (*.cfg)"));
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(buffer.GetString());
    file.close();
}

//    connect(ui->moneyCountSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int _value) { });
//    connect(ui->rankSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int _value) { });
//    connect(ui->experienceCountSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int _value) { });
