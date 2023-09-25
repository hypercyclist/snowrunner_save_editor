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

    ui->completeTasksTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    updateTasksTable();
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

void EditorWindow::updateTasksTable()
{
    ui->regionFilterCombobox->clear();
    ui->mapFilterCombobox->clear();

    QAbstractItemModel* tasksTableModel = ui->completeTasksTable->model();

    QMap<QString, QMap<QString, QVector<QString>>>& tasks = m_tasks->tasks();

    auto regionIt = tasks.begin();
    while(regionIt != tasks.end())
    {
        ui->regionFilterCombobox->addItem(regionIt->firstKey());
        QMap<QString, QVector<QString>>& maps = regionIt.value();
        auto mapsIt = maps.begin();
        while (mapsIt != maps.end())
        {
            ui->mapFilterCombobox->addItem(mapsIt->first());
            for (QString taskCode : mapsIt.value())
            {
                int rowIndex = ui->completeTasksTable->rowCount();
                ui->completeTasksTable->insertRow(rowIndex);

                QString region = m_localization->getLocalization(regionIt->firstKey() + "_DESC");

                QString map = m_localization->getLocalization(mapsIt->first() + "_NAME");
                if (map == "No translation")
                    map = m_localization->getLocalization(mapsIt->first() + "_NEW_NAME");
                if (map == "No translation")
                    map = m_localization->getLocalization("LEVEL_" + mapsIt->first() + "_NAME");
                if (map == "No translation")
                    map = m_localization->getLocalization("LEVEL_" + mapsIt->first());

                QString task = m_localization->getLocalization(taskCode);

                tasksTableModel->setData(tasksTableModel->index(rowIndex, 0), region);
                tasksTableModel->setData(tasksTableModel->index(rowIndex, 1), map);
                tasksTableModel->setData(tasksTableModel->index(rowIndex, 2), task);

                QCheckBox* statusCheckBox = new QCheckBox();
                QWidget* statusContainer = new QWidget();
                QHBoxLayout* statusContainerLayout = new QHBoxLayout();
                statusContainerLayout->setContentsMargins(0, 0, 0, 0);
                statusContainerLayout->setAlignment( Qt::AlignCenter );
                statusContainerLayout->addWidget(statusCheckBox);
                statusContainer->setLayout(statusContainerLayout);
                ui->completeTasksTable->setCellWidget(rowIndex, 3, statusContainer);
            }
            mapsIt++;
        }
        regionIt++;
    }
}

















