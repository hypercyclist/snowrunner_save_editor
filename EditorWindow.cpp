#include "EditorWindow.h"
#include "ui_EditorWindow.h"

#include <QCheckBox>
#include <QDebug>
#include <QFileDialog>
#include <QAbstractItemModel>
#include "Localization.h"
#include "Tasks.h"
#include <rapidjson/writer.h>

EditorWindow::EditorWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::EditorWindow),
    m_localization(nullptr),
    m_tasks(nullptr),
    text("")
{
    ui->setupUi(this);
    m_localization = new Localization();
    m_localization->loadLocalizations();
    m_localization->setDefaultLanguage(Language::RUSSIAN);

    m_tasks = new Tasks();
    m_tasks->loadTasksData();

    ui->completeTasksTable->setRegionFilterCombobox(ui->regionFilterCombobox);
    ui->completeTasksTable->setMapFilterCombobox(ui->mapFilterCombobox);
    ui->completeTasksTable->setFilterApplyButton(ui->filterApplyButton);
    ui->completeTasksTable->filterMaps();
    ui->completeTasksTable->updateTasksTable();
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

    rapidjson::Value& persistentProfileData =
        jsonDocument["CompleteSave"]["SslValue"]["persistentProfileData"];

    ui->moneyCountSpinBox->setValue(persistentProfileData["money"].GetInt());
    ui->rankSpinBox->setValue(persistentProfileData["rank"].GetInt());
    ui->experienceCountSpinBox->setValue(persistentProfileData["experience"].GetInt());

    updateTasksTable();

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

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    jsonDocument.Accept(writer);

    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"), "", tr("CFG (*.cfg)"));
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(buffer.GetString());
    file.close();
}
