#include "EditorWindow.h"
#include "ui_EditorWindow.h"

#include <QCheckBox>
#include <QDebug>
#include <QFileDialog>
#include <QAbstractItemModel>
#include "Localization.h"
#include "Database.h"
#include <rapidjson/writer.h>

#include <fstream>
#include <iostream>

EditorWindow::EditorWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::EditorWindow),
    m_database(nullptr),
    text("")
{
    ui->setupUi(this);

    m_database = new Database();
    m_database->createDatabase();
    m_database->loadDatabase();

//    ui->completeTasksTable->setLocalization(m_localization);
//    ui->completeTasksTable->setTasks();
//    ui->completeTasksTable->setRegionFilterCombobox(ui->regionFilterCombobox);
//    ui->completeTasksTable->setMapFilterCombobox(ui->mapFilterCombobox);
//    ui->completeTasksTable->setFilterApplyButton(ui->filterApplyButton);
//    ui->completeTasksTable->setCheckAllFilteredButton(ui->checkAllFilteredButton);
//    ui->completeTasksTable->filterMaps();
//    ui->completeTasksTable->updateTasksTable();
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

    QVector<QString> finishedObjs;
    rapidjson::Value& finishedObjsArray = sslValue["finishedObjs"].GetArray();
    for (unsigned int i = 0; i < finishedObjsArray.Size(); i++)
    {
        finishedObjs.push_back(finishedObjsArray[i].GetString());
    }
    ui->completeTasksTable->setCompleteFromVector(finishedObjs);
    ui->completeTasksTable->updateTasksTable();


//    connect(ui->moneyCountSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int _value) { });
//    connect(ui->rankSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int _value) { });
//    connect(ui->experienceCountSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int _value) { });
}

void EditorWindow::on_menuSave_triggered()
{
//    rapidjson::GenericObject& persistentProfileData =
//        jsonDocument["CompleteSave"]["SslValue"]["persistentProfileData"].GetObject();
//    qDebug() << "test";
    jsonDocument["CompleteSave"]["SslValue"]["persistentProfileData"]["money"] = ui->moneyCountSpinBox->value();
    jsonDocument["CompleteSave"]["SslValue"]["persistentProfileData"]["rank"] = ui->rankSpinBox->value();
    jsonDocument["CompleteSave"]["SslValue"]["persistentProfileData"]["experience"] = ui->experienceCountSpinBox->value();
    rapidjson::Value finishedObjsArray;
    finishedObjsArray.SetArray();
    for (const QString& taskCode : ui->completeTasksTable->completedTasks())
    {
        rapidjson::Value strVal;
        strVal.SetString(taskCode.toStdString().c_str(), taskCode.length(), jsonDocument.GetAllocator());
        finishedObjsArray.PushBack(strVal, jsonDocument.GetAllocator());
    }
    jsonDocument["CompleteSave"]["SslValue"]["finishedObjs"] = finishedObjsArray;

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    jsonDocument.Accept(writer);

    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"), "", tr("CFG (*.cfg)"));
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(buffer.GetString());
    file.close();
}
