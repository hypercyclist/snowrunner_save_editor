#include "EditorWindow.h"
#include "ui_EditorWindow.h"

#include <QCheckBox>
#include <QDebug>
#include <QFileDialog>
#include <QAbstractItemModel>
#include "Localization.h"
#include "Database.h"
#include "GameAtlas.h"
#include "Task.h"
#include "Upgrade.h"
#include "Utils.h"
#include "TableFilterByRegionMap.h"
#include "ui_TableFilterByRegionMap.h"
#include "InformationWidget.h"
#include <fstream>
#include <iostream>
#include "SaveFile.h"
#include <QMessageBox>
#include "AppConfig.h"
#include <QFileInfo>

EditorWindow::EditorWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::EditorWindow),
    m_appConfig(nullptr),
    m_database(nullptr),
    m_saveFile(nullptr),
    text("")
{
    ui->setupUi(this);

    std::string appFolderPath = QDir::currentPath().toStdString();
    std::string configFullFileName = appFolderPath + "/config.json";

    m_appConfig = new AppConfig();
    m_appConfig->loadConfig(configFullFileName);

    std::string databasePath = m_appConfig->databaseFolderPath();

    m_database = new Database();
    bool databaseLoaded = m_database->loadDatabase(databasePath);
    if (!databaseLoaded)
    {
        m_database->createDatabase(databasePath);
    }

    configureTasksTable();
    configureUpgradesTable();

    applyLanguage(m_database->localization()->languageByTextName(
        m_appConfig->lastLanguageCode()));

    connectLanguageButtonsToFunctions();

// Disable tabs before save will open.
    setTabsEnabled(false);
}

EditorWindow::~EditorWindow()
{
    delete ui;

    if (m_database) delete m_database;
    if (m_saveFile) delete m_saveFile;
}

void EditorWindow::on_menuOpen_triggered()
{   
// Open and load file.
    std::string fullFileName = QFileDialog::getOpenFileName(this,
        tr("Открыть файл"),
        QString::fromStdString(m_appConfig->lastSaveFolderPath()),
        tr("SAVEFILE (*.cfg *.dat)")).toStdString();

    if (fullFileName.size() == 0)
    {
        return;
    }

    if (m_saveFile)
    {
        delete m_saveFile;
    }
    m_saveFile = new SaveFile();
    if (!m_saveFile->loadSaveFile(fullFileName))
    {
        QMessageBox* mb = new QMessageBox();
        mb->setWindowTitle("Save file loading error");
        mb->setText(QString::fromStdString(m_saveFile->error()));
        mb->exec();
        return;
    }

    QFileInfo fileInfo(QString::fromStdString(fullFileName));
    m_appConfig->setLastSaveFolderPath(fileInfo.absolutePath().toStdString());
    m_appConfig->saveConfig();

// Enable tabs after loading.
    setTabsEnabled(true);

// Grab values.
    ui->moneyCountSpinBox->setValue(m_saveFile->m_money);
    ui->rankSpinBox->setValue(m_saveFile->m_rank);
    ui->experienceCountSpinBox->setValue(m_saveFile->m_experience);

    ui->gameDifficultyModeCombobox->setCurrentIndex(m_saveFile->m_gameDifficultyMode);
    ui->isHardModeCheckbox->setChecked(m_saveFile->m_isHardMode);

    m_database->gameAtlas()->setTasksCompleteFromVectorCodes(m_saveFile->m_finishedObjs);
    ui->completeTasksTable->updateTable();

    m_database->gameAtlas()->setUpgradesReceivedFromVectorCodes(m_saveFile->m_receivedUpgrades);
    ui->upgradesTable->updateTable();
}

void EditorWindow::on_menuSave_triggered()
{
    if (!m_saveFile || !m_saveFile->loaded())
    {
        return;
    }

// Сhoose file destination and name. It can create and override file.
    std::string fullFileName = QFileDialog::getSaveFileName(this,
        tr("Сохранить файл"),
        QString::fromStdString(m_appConfig->lastSaveFolderPath()),
        tr("SAVEFILE (*.cfg *.dat)")).toStdString();

    if (fullFileName.size() == 0)
    {
        return;
    }

//Apply values.
    m_saveFile->m_money = ui->moneyCountSpinBox->value();
    m_saveFile->m_rank = ui->rankSpinBox->value();
    m_saveFile->m_experience = ui->experienceCountSpinBox->value();

    m_saveFile->m_gameDifficultyMode = ui->gameDifficultyModeCombobox->currentIndex();
    m_saveFile->m_isHardMode = ui->isHardModeCheckbox->isChecked();

    std::vector<std::string> finishedObjs;
    for (Task* task : m_database->gameAtlas()->completedTasks())
    {
        finishedObjs.push_back(task->code());
    }
    m_saveFile->m_finishedObjs = finishedObjs;

    std::vector<std::string> receivedUpgrades;
    for (Upgrade* upgrade : m_database->gameAtlas()->receivedUpgrades())
    {
        receivedUpgrades.push_back(upgrade->code());
    }
    m_saveFile->m_receivedUpgrades = receivedUpgrades;

// Save file.
    if (!m_saveFile->saveSaveFile(fullFileName))
    {
        QMessageBox* mb = new QMessageBox();
        mb->setWindowTitle("Save file saving error");
        mb->setText(QString::fromStdString(m_saveFile->error()));
        mb->exec();
        return;
    }
}

// This function change texts of GUI elements.
void EditorWindow::applyLanguage(Language _language)
{
    Localization* localization = m_database->localization();
    localization->setDefaultLanguage(_language);

    m_appConfig->setLastLanguageCode(localization->languageTextName(_language));
    m_appConfig->saveConfig();

    ui->menu->setTitle(localization->getLocalization("UI_INGAME_MENU").c_str());
    ui->menuOpen->setText(localization->getLocalization("UI_POLYGON_PACKER_DIALOG_OPEN").c_str());
    ui->menuSave->setText(localization->getLocalization("UI_LOG_SAVE_IN_PROGRESS").c_str());

    ui->menuAbout->setTitle(localization->getLocalization("UI_POLYGON_INFO").c_str());
    ui->menuInformation->setText(localization->getLocalization("UI_POLYGON_INFO").c_str());

    ui->menuLanguage->setToolTip(localization->getLocalization("UI_SETTINGS_LANGUAGE").c_str());

    ui->tabWidget->setTabText(0, localization->getLocalization("UI_OPEN_PROFILE").c_str());
    ui->moneyCountLabel->setText(localization->getLocalization("UI_POPUP_DEPLOY_MONEY").c_str());

    std::string rankLabelString = localization->getLocalization("UI_NGP_RANK_10");
    rankLabelString = Utils::replace(rankLabelString, "10", "");
    rankLabelString = Utils::replace(rankLabelString, " ", "");
    ui->rankLabel->setText(rankLabelString.c_str());
    ui->experienceCountLabel->setText(localization->getLocalization("CODEX_LEVEL_UP_HEADER").c_str());

    std::string gameString = localization->getLocalization("UI_SETTINGS_GAME");
    ui->tabWidget->setTabText(1, gameString.c_str());
    ui->gameDifficultyModeLabel->setText(gameString.c_str());

    ui->gameDifficultyModeCombobox->clear();
    ui->gameDifficultyModeCombobox->addItem(localization->getLocalization("UI_MAIN_MENU_NEW_GAME_LIST_USUAL").c_str());
    ui->gameDifficultyModeCombobox->addItem(localization->getLocalization("UI_HARDMODE").c_str());
    ui->gameDifficultyModeCombobox->addItem(localization->getLocalization("UI_NGPLUS").c_str());
    if (m_saveFile)
    {
        ui->gameDifficultyModeCombobox->setCurrentIndex(m_saveFile->m_gameDifficultyMode);
    }

    ui->isHardModeLabel->setText(localization->getLocalization("UI_HARDMODE").c_str());
    ui->newGamePlusSettingsLabel->setText(localization->getLocalization("NEW_GAME_PLUS_HEADER").c_str());

    std::string tasksAndContractsString =
        localization->getLocalization("UI_PLAYER_PROFILE_TAB_TASKS_NAME") + " / " +
        localization->getLocalization("UI_PLAYER_PROFILE_TAB_CONTRACTS_NAME");
    ui->tabWidget->setTabText(2, tasksAndContractsString.c_str());

    std::string regionString = localization->getLocalization("UI_MM_SETTINGS_REGION");
    std::string mapString = localization->getLocalization("UI_MINIMAP");
    std::string refreshString = localization->getLocalization("UI_REFRESH");
    ui->completeTasksTableBar->gui()->regionFilterLabel->setText(regionString.c_str());
    ui->completeTasksTableBar->gui()->mapFilterLabel->setText(mapString.c_str());
    ui->completeTasksTableBar->gui()->filterApplyButton->setText(refreshString.c_str());

    std::string checkAllFilteredString =
        localization->getLocalization("UI_MOD_BROWSER_MORE_OPTIONS_DISABLE_ALL") + " / " +
        localization->getLocalization("UI_MOD_BROWSER_MORE_OPTIONS_ENABLE_ALL");
    ui->completeTasksTableBar->gui()->checkAllFilteredButton->setText(checkAllFilteredString.c_str());

    ui->completeTasksTable->horizontalHeaderItem(0)->setText(regionString.c_str());
    ui->completeTasksTable->horizontalHeaderItem(1)->setText(mapString.c_str());

    std::string taskOrContractString =
            localization->getLocalization("UI_HUD_NAV_PANEL_SHOW_TASK") + " / " +
            localization->getLocalization("UI_HUD_EVENT_DISCOVERED_OBJECTIVE_CONTRACT");
    ui->completeTasksTable->horizontalHeaderItem(2)->setText(taskOrContractString.c_str());

    std::string selectString = localization->getLocalization("UI_SELECT");
    ui->completeTasksTable->horizontalHeaderItem(3)->setText(selectString.c_str());

    ui->tabWidget->setTabText(3, localization->getLocalization("UI_GARAGE_MODIFICATIONS_UPGRADES").c_str());

    ui->upgradesTableBar->gui()->regionFilterLabel->setText(regionString.c_str());
    ui->upgradesTableBar->gui()->mapFilterLabel->setText(mapString.c_str());
    ui->upgradesTableBar->gui()->filterApplyButton->setText(refreshString.c_str());
    ui->upgradesTableBar->gui()->checkAllFilteredButton->setText(checkAllFilteredString.c_str());

    ui->upgradesTable->horizontalHeaderItem(0)->setText(regionString.c_str());
    ui->upgradesTable->horizontalHeaderItem(1)->setText(mapString.c_str());
    ui->upgradesTable->horizontalHeaderItem(2)->setText(localization->getLocalization("UI_HUD_NAV_PANEL_GET_UPGRADE").c_str());
    ui->upgradesTable->horizontalHeaderItem(3)->setText(selectString.c_str());

    ui->completeTasksTableBar->filterMaps();
    ui->completeTasksTable->updateTable();

    ui->upgradesTableBar->filterMaps();
    ui->upgradesTable->updateTable();
}

// Open information window.
void EditorWindow::on_menuInformation_triggered()
{
    InformationWidget* informationWidget = new InformationWidget(this);
    informationWidget->setWindowTitle(m_database->localization()->
        getLocalization("UI_POLYGON_INFO").c_str());
    informationWidget->setWindowFlags(Qt::Window);
    informationWidget->setWindowModality(Qt::WindowModality::ApplicationModal);
    informationWidget->setAttribute(Qt::WA_DeleteOnClose);
    informationWidget->show();
}

void EditorWindow::setTabsEnabled(bool _state)
{
    for (int i = 0; i < ui->tabWidget->count(); i++)
    {
        ui->tabWidget->widget(i)->setEnabled(_state);
    }
}

void EditorWindow::configureTasksTable()
{
    ui->completeTasksTable->setGameAtlas(m_database->gameAtlas());
    ui->completeTasksTable->setLocalization(m_database->localization());
    ui->completeTasksTable->setFilterBarWidget(ui->completeTasksTableBar);
    ui->completeTasksTableBar->setLocalization(m_database->localization());
    ui->completeTasksTableBar->setGameAtlas(m_database->gameAtlas());
    ui->completeTasksTableBar->filterMaps();
    ui->completeTasksTable->updateTable();
}

void EditorWindow::configureUpgradesTable()
{
    ui->upgradesTable->setGameAtlas(m_database->gameAtlas());
    ui->upgradesTable->setLocalization(m_database->localization());
    ui->upgradesTable->setFilterBarWidget(ui->upgradesTableBar);
    ui->upgradesTableBar->setLocalization(m_database->localization());
    ui->upgradesTableBar->setGameAtlas(m_database->gameAtlas());
    ui->upgradesTableBar->filterMaps();
    ui->upgradesTable->updateTable();
}

void EditorWindow::connectLanguageButtonsToFunctions()
{
    connect(ui->menuRussian, &QAction::triggered, this, [=] { applyLanguage(Language::RUSSIAN); });
    connect(ui->menuChineseTraditional, &QAction::triggered, this, [=] { applyLanguage(Language::CHINESE_TRADITIONAL); });
    connect(ui->menuChineseSimple, &QAction::triggered, this, [=] { applyLanguage(Language::CHINESE_SIMPLIFIED); });
    connect(ui->menuBrazilian, &QAction::triggered, this, [=] { applyLanguage(Language::BRAZILIAN_PORTUGUESE); });
    connect(ui->menuKorean, &QAction::triggered, this, [=] { applyLanguage(Language::KOREAN); });
    connect(ui->menuEnglish, &QAction::triggered, this, [=] { applyLanguage(Language::ENGLISH); });
    connect(ui->menuJapanese, &QAction::triggered, this, [=] { applyLanguage(Language::JAPANESE); });
    connect(ui->menuGerman, &QAction::triggered, this, [=] { applyLanguage(Language::GERMAN); });
    connect(ui->menuSpanish, &QAction::triggered, this, [=] { applyLanguage(Language::SPANISH); });
    connect(ui->menuFrench, &QAction::triggered, this, [=] { applyLanguage(Language::FRENCH); });
    connect(ui->menuCzech, &QAction::triggered, this, [=] { applyLanguage(Language::CZECH); });
    connect(ui->menuItalian, &QAction::triggered, this, [=] { applyLanguage(Language::ITALIAN); });
    connect(ui->menuPolish, &QAction::triggered, this, [=] { applyLanguage(Language::POLISH); });
}

//    connect(ui->moneyCountSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int _value) { });
//    connect(ui->rankSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int _value) { });
//    connect(ui->experienceCountSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int _value) { });
