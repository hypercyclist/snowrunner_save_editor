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
#include "Upgrade.h"
#include "Utils.h"
#include "TableFilterByRegionMap.h"
#include "ui_TableFilterByRegionMap.h"

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
    ui->completeTasksTable->setLocalization(m_database->localization());
    ui->completeTasksTable->setFilterBarWidget(ui->completeTasksTableBar);
    ui->completeTasksTableBar->setGameAtlas(m_database->gameAtlas());
    ui->completeTasksTableBar->filterMaps();
    ui->completeTasksTable->updateTable();

    ui->upgradesTable->setGameAtlas(m_database->gameAtlas());
    ui->upgradesTable->setLocalization(m_database->localization());
    ui->upgradesTable->setFilterBarWidget(ui->completeTasksTableBar);
    ui->upgradesTableBar->setGameAtlas(m_database->gameAtlas());
    ui->upgradesTableBar->filterMaps();
    ui->upgradesTable->updateTable();

    connect(ui->menuRussian, &QAction::triggered, this, [=] { applyLanguage(Language::RUSSIAN); });
    connect(ui->menuEnglish, &QAction::triggered, this, [=] { applyLanguage(Language::ENGLISH); });
    connect(ui->menuChineseTraditional, &QAction::triggered, this, [=] { applyLanguage(Language::CHINESE_TRADITIONAL); });
    connect(ui->menuChineseSimple, &QAction::triggered, this, [=] { applyLanguage(Language::CHINESE_SIMPLE); });
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
    ui->completeTasksTable->updateTable();

    std::vector<std::string> receivedUpgrades;
    rapidjson::Value& receivedUpgradesValue = sslValue["upgradesGiverData"];
    // for (auto& m : document.GetObject())
    for (auto levelIt = receivedUpgradesValue.MemberBegin(); levelIt != receivedUpgradesValue.MemberEnd(); levelIt++)
    {
        for (auto upgradesIt = levelIt->value.MemberBegin(); upgradesIt != levelIt->value.MemberEnd(); upgradesIt++)
        {
            // Если открыли файл заново, то все смешается.
            if (upgradesIt->value.GetInt() == 2)
            {
                receivedUpgrades.push_back(upgradesIt->name.GetString());
            }
        }
    }
    m_database->gameAtlas()->setUpgradesReceivedFromVectorCodes(receivedUpgrades);
    ui->upgradesTable->updateTable();
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

    rapidjson::Value& receivedUpgradesValue = jsonDocument["CompleteSave"]["SslValue"]["upgradesGiverData"];
    std::vector<Upgrade*> newReceivedUpgrades = m_database->gameAtlas()->receivedUpgrades();
    for (auto& levelObject : receivedUpgradesValue.GetObject())
    {
        for (auto& upgradeString : levelObject.value.GetObject())
        {
            bool found = false; // Хрень какая-то.
            for (Upgrade* upgrade : newReceivedUpgrades)
            {
                if (upgradeString.name.GetString() == upgrade->code())
                {
                    upgradeString.value = 2;
                    found = true;
                }
            }
            if (!found)
            {
                upgradeString.value = 0;
            }
        }
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    jsonDocument.Accept(writer);

    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"), "", tr("CFG (*.cfg)"));
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(buffer.GetString());
    file.close();
}

void EditorWindow::applyLanguage(Language _language)
{
    Localization* localization = m_database->localization();
    ui->menu->setTitle(localization->getLocalization("UI_INGAME_MENU", _language).c_str());
    ui->menuOpen->setText(localization->getLocalization("UI_POLYGON_PACKER_DIALOG_OPEN", _language).c_str());
    ui->menuSave->setText(localization->getLocalization("UI_LOG_SAVE_IN_PROGRESS", _language).c_str());

    ui->menuLanguage->setToolTip(localization->getLocalization("UI_SETTINGS_LANGUAGE", _language).c_str());

    ui->tabWidget->setTabText(0, localization->getLocalization("UI_OPEN_PROFILE", _language).c_str());
    ui->moneyCountLabel->setText(localization->getLocalization("UI_POPUP_DEPLOY_MONEY", _language).c_str());

    std::string rankLabelString = localization->getLocalization("UI_NGP_RANK_10", _language);
    rankLabelString = Utils::replace(rankLabelString, "10", "");
    rankLabelString = Utils::replace(rankLabelString, " ", "");
    ui->rankLabel->setText(rankLabelString.c_str());
    ui->experienceCountLabel->setText(localization->getLocalization("CODEX_LEVEL_UP_HEADER", _language).c_str());

    std::string tasksAndContractsString =
        localization->getLocalization("UI_PLAYER_PROFILE_TAB_TASKS_NAME", _language) + "/" +
        localization->getLocalization("UI_PLAYER_PROFILE_TAB_CONTRACTS_NAME", _language);
    ui->tabWidget->setTabText(1, tasksAndContractsString.c_str());

    std::string regionString = localization->getLocalization("UI_MM_SETTINGS_REGION", _language);
    std::string mapString = localization->getLocalization("UI_MINIMAP", _language);
    std::string refreshString = localization->getLocalization("UI_REFRESH", _language);
    ui->completeTasksTableBar->gui()->regionFilterLabel->setText(regionString.c_str());
    ui->completeTasksTableBar->gui()->mapFilterLabel->setText(mapString.c_str());
    ui->completeTasksTableBar->gui()->filterApplyButton->setText(refreshString.c_str());

    std::string checkAllFilteredString =
        localization->getLocalization("UI_MOD_BROWSER_MORE_OPTIONS_ENABLE_ALL", _language) + "/" +
        localization->getLocalization("UI_MOD_BROWSER_MORE_OPTIONS_DISABLE_ALL", _language);
    ui->completeTasksTableBar->gui()->checkAllFilteredButton->setText(checkAllFilteredString.c_str());

    ui->completeTasksTable->horizontalHeaderItem(0)->setText(regionString.c_str());
    ui->completeTasksTable->horizontalHeaderItem(1)->setText(mapString.c_str());

    std::string taskOrContractString =
            localization->getLocalization("UI_HUD_NAV_PANEL_SHOW_TASK", _language) + "/" +
            localization->getLocalization("UI_HUD_EVENT_DISCOVERED_OBJECTIVE_CONTRACT", _language);
    ui->completeTasksTable->horizontalHeaderItem(2)->setText(taskOrContractString.c_str());

    std::string selectString = localization->getLocalization("UI_SELECT", _language);
    ui->completeTasksTable->horizontalHeaderItem(3)->setText(selectString.c_str());

    ui->tabWidget->setTabText(2, localization->getLocalization("UI_GARAGE_MODIFICATIONS_UPGRADES", _language).c_str());

    ui->upgradesTableBar->gui()->regionFilterLabel->setText(regionString.c_str());
    ui->upgradesTableBar->gui()->mapFilterLabel->setText(mapString.c_str());
    ui->upgradesTableBar->gui()->filterApplyButton->setText(refreshString.c_str());
    ui->upgradesTableBar->gui()->checkAllFilteredButton->setText(checkAllFilteredString.c_str());

    ui->upgradesTable->horizontalHeaderItem(0)->setText(regionString.c_str());
    ui->upgradesTable->horizontalHeaderItem(1)->setText(mapString.c_str());
    ui->upgradesTable->horizontalHeaderItem(2)->setText(localization->getLocalization("UI_HUD_NAV_PANEL_GET_UPGRADE", _language).c_str());
    ui->upgradesTable->horizontalHeaderItem(3)->setText(selectString.c_str());

    ui->tabWidget->setTabText(3, localization->getLocalization("UI_PLAYER_PROFILE_STATISTICS_HEADER", _language).c_str());

    localization->setDefaultLanguage(_language);

    ui->completeTasksTableBar->filterMaps();
    ui->completeTasksTable->updateTable();

    ui->upgradesTableBar->filterMaps();
    ui->upgradesTable->updateTable();
}

//    connect(ui->moneyCountSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int _value) { });
//    connect(ui->rankSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int _value) { });
//    connect(ui->experienceCountSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int _value) { });
