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

    configureNgpRulesTable();
    configureTasksTable();
    configureUpgradesTable();

    applyLanguage(m_database->localization()->languageByTextName(
        m_appConfig->lastLanguageCode()));

    connectLanguageButtonsToFunctions();

// Disable tabs before save will open.
//    setTabsEnabled(false);
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
    ui->moneyCountSpinBox->setValue(m_saveFile->money());
    ui->rankSpinBox->setValue(m_saveFile->rank());
    ui->experienceCountSpinBox->setValue(m_saveFile->experience());

    ui->gameDifficultyModeCombobox->setCurrentIndex(m_saveFile->gameDifficultyMode());
    ui->isHardModeCheckbox->setChecked(m_saveFile->isHardMode());

    m_database->gameAtlas()->setTasksCompleteFromVectorCodes(m_saveFile->finishedObjs());
    ui->completeTasksTable->updateTable();

    m_database->gameAtlas()->setUpgradesReceivedFromVectorCodes(m_saveFile->receivedUpgrades());
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
    m_saveFile->setMoney(ui->moneyCountSpinBox->value());
    m_saveFile->setRank(ui->rankSpinBox->value());
    m_saveFile->setExperience(ui->experienceCountSpinBox->value());

    m_saveFile->setGameDifficultyMode(ui->gameDifficultyModeCombobox->currentIndex());
    m_saveFile->setIsHardMode(ui->isHardModeCheckbox->isChecked());

    std::vector<std::string> finishedObjs;
    for (Task* task : m_database->gameAtlas()->completedTasks())
    {
        finishedObjs.push_back(task->code());
    }
    m_saveFile->setFinishedObjs(finishedObjs);

    std::vector<std::string> receivedUpgrades;
    for (Upgrade* upgrade : m_database->gameAtlas()->receivedUpgrades())
    {
        receivedUpgrades.push_back(upgrade->code());
    }
    m_saveFile->setReceivedUpgrades(receivedUpgrades);

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
        ui->gameDifficultyModeCombobox->setCurrentIndex(m_saveFile->gameDifficultyMode());
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

void EditorWindow::configureNgpRulesTable()
{
    ui->truckAvailabilityLabel->setText("UI_TRUCK_AVAILABILITY");
    ui->truckAvailabilityCombo->addItem("UI_NGP_DEFAULT");
    ui->truckAvailabilityCombo->addItem("UI_NGP_ALL_TRUCKS_FROM_START");
    ui->truckAvailabilityCombo->addItem("UI_NGP_RANDOM_TRUCKS_IN_EVERY_GARAGE");
    ui->truckAvailabilityCombo->addItem("UI_NGP_TRUCK_STORE_FROM_LVL_10");
    ui->truckAvailabilityCombo->addItem("UI_NGP_TRUCK_STORE_FROM_LVL_20");
    ui->truckAvailabilityCombo->addItem("UI_NGP_TRUCK_STORE_FROM_LVL_30");
    ui->truckAvailabilityCombo->addItem("UI_NGP_UNAVAILABLE_TRUCK_STORE");
    ui->truckAvailabilityCombo->addItem("UI_NGP_RANDOM_OPTION");

    ui->dlcVehiclesAvailabilityLabel->setText("UI_DLC_VEHICLES");
    ui->dlcVehiclesAvailabilityCombo->addItem("UI_NGP_AVAILABLE");
    ui->dlcVehiclesAvailabilityCombo->addItem("UI_NGP_UNAVAILABLE");
    ui->dlcVehiclesAvailabilityCombo->addItem("UI_NGP_RANDOM_OPTION");

    ui->truckPricingLabel->setText("UI_TRUCK_PRICING");
    ui->truckPricingCombo->addItem("UI_NGP_DEFAULT");
    ui->truckPricingCombo->addItem("UI_NGP_FREE_TRUCKS");
    ui->truckPricingCombo->addItem("UI_NGP_X2_PRICES_TRUCKS");
    ui->truckPricingCombo->addItem("UI_NGP_X4_PRICES_TRUCKS");
    ui->truckPricingCombo->addItem("UI_NGP_X6_PRICES_TRUCKS");
    ui->truckPricingCombo->addItem("UI_NGP_RANDOMIZED_X1_TO_X6_TRUCKS");
    ui->truckPricingCombo->addItem("UI_NGP_RANDOM_OPTION");

    ui->truckSellingLabel->setText("UI_NGP_TRUCK_SELLING");
    ui->truckSellingCombo->addItem("UI_NGP_DEFAULT");
    ui->truckSellingCombo->addItem("UI_NGP_0.5X_PRICES_TRUCKS");
    ui->truckSellingCombo->addItem("UI_NGP_0.3X_PRICES_TRUCKS");
    ui->truckSellingCombo->addItem("UI_NGP_0.1X_PRICES_TRUCKS");
    ui->truckSellingCombo->addItem("UI_NGP_UNABLE_TO_SELL_TRUCKS");
    ui->truckSellingCombo->addItem("UI_NGP_RANDOM_OPTION");

    ui->vehicleStorageAvailabilityLabel->setText("UI_VEHICLE_STORAGE_AVAILABILITY");
    ui->vehicleStorageAvailabilityCombo->addItem("UI_NGP_DEFAULT");
    ui->vehicleStorageAvailabilityCombo->addItem("UI_NGP_3_SLOTS");
    ui->vehicleStorageAvailabilityCombo->addItem("UI_NGP_5_SLOTS");
    ui->vehicleStorageAvailabilityCombo->addItem("UI_NGP_10_SLOTS");
    ui->vehicleStorageAvailabilityCombo->addItem("UI_NGP_ONLY_SCOUT_VEHICLES");
    ui->vehicleStorageAvailabilityCombo->addItem("UI_NGP_RANDOM_OPTION");

    ui->trailerAvailabilityLabel->setText("UI_NGP_TRALER_AVAILABILITY");
    ui->trailerAvailabilityCombo->addItem("UI_NGP_DEFAULT");
    ui->trailerAvailabilityCombo->addItem("UI_NGP_ALL_TRAILERS_AVAILABLE");
    ui->trailerAvailabilityCombo->addItem("UI_NGP_ONLY_FOUND_IN_WORLD");
    ui->trailerAvailabilityCombo->addItem("UI_NGP_RANDOM_OPTION");

    ui->trailerStoreAvailabilityLabel->setText("UI_NGP_TRAILER_STORE_AVAILABILITY");
    ui->trailerStoreAvailabilityCombo->addItem("UI_NGP_DEFAULT");
    ui->trailerStoreAvailabilityCombo->addItem("UI_NGP_ONLY_FOUND_IN_WORLD");
    ui->trailerStoreAvailabilityCombo->addItem("UI_NGP_RANDOM_OPTION");

    ui->trailerPricingLabel->setText("UI_NP_TRAILER_PRICING");
    ui->trailerPricingCombo->addItem("UI_NGP_DEFAULT");
    ui->trailerPricingCombo->addItem("UI_NGP_ALL_TRAILERS_FREE");
    ui->trailerPricingCombo->addItem("UI_NGP_X2_PRICES_TRAILERS");
    ui->trailerPricingCombo->addItem("UI_NGP_X4_PRICES_TRAILERS");
    ui->trailerPricingCombo->addItem("UI_NGP_X6_PRICES_TRAILERS");
    ui->trailerPricingCombo->addItem("UI_NGP_RANDOMIZED_X1_TO_X6_TRAILERS");
    ui->trailerPricingCombo->addItem("UI_NGP_RANDOM_OPTION");

    ui->trailerSellingPriceLabel->setText("UI_NGP_TRAILER_SELLING");
    ui->trailerSellingPriceCombo->addItem("UI_NGP_DEFAULT");
    ui->trailerSellingPriceCombo->addItem("UI_NGP_0.5X_PRICES_TRAILERS");
    ui->trailerSellingPriceCombo->addItem("UI_NGP_0.3X_PRICES_TRAILERS");
    ui->trailerSellingPriceCombo->addItem("UI_NGP_0.1X_PRICES_TRAILERS");
    ui->trailerSellingPriceCombo->addItem("UI_NGP_UNABLE_TO_SELL_TRAILERS");
    ui->trailerSellingPriceCombo->addItem("UI_NGP_RANDOM_OPTION");

    ui->externalBodyAddonsAvailabilityLabel->setText("UI_NGP_ALL_ADDONS_AVAILABLE");
    ui->externalBodyAddonsAvailabilityCombo->addItem("UI_NGP_DEFAULT");
    ui->externalBodyAddonsAvailabilityCombo->addItem("UI_NGP_ALL_ADDONS_AVAILABLE");
    ui->externalBodyAddonsAvailabilityCombo->addItem("UI_NGP_RAND_SET_OF_5_ADDONS");
    ui->externalBodyAddonsAvailabilityCombo->addItem("UI_NGP_RAND_SET_OF_10_ADDONS");
    ui->externalBodyAddonsAvailabilityCombo->addItem("UI_NGP_RAND_SET_OF_10_IN_EVERY_GARAGE_ADDONS");
    ui->externalBodyAddonsAvailabilityCombo->addItem("UI_NGP_RANDOM_OPTION");

    ui->tireAvailAbilityLabel->setText("UI_TYRE_AVAILABILITY");
    ui->tireAvailAbilityCombo->addItem("UI_NGP_DEFAULT");
    ui->tireAvailAbilityCombo->addItem("UI_NGP_ALL_TYRES_AVAILABLE");
    ui->tireAvailAbilityCombo->addItem("UI_NGP_ONLY_HIGHWAY_AND_ALLROAD");
    ui->tireAvailAbilityCombo->addItem("UI_NGP_ONLY_HIGHWAY_AND_ALLROAD_AND_OFFROAD");
    ui->tireAvailAbilityCombo->addItem("UI_NGP_NO_MUD_TYRES");
    ui->tireAvailAbilityCombo->addItem("UI_NGP_NO_CHAINED_TYRES");
    ui->tireAvailAbilityCombo->addItem("UI_NGP_SET_OF_RANDOM_IN_EVETY_GARAGE");
    ui->tireAvailAbilityCombo->addItem("UI_NGP_RANDOM_OPTION");

    ui->internalAddonAvailabilityLabel->setText("UI_INTERNAL_ADDON_AVAILABILITY");
    ui->internalAddonAvailabilityCombo->addItem("UI_NGP_DEFAULT");
    ui->internalAddonAvailabilityCombo->addItem("UI_NGP_ALL_INTERNAL_ADDONS_AVAILABLE"); // Need to check!
    ui->internalAddonAvailabilityCombo->addItem("UI_NGP_RAND_AMOUNT_IN_10_TO_50");
    ui->internalAddonAvailabilityCombo->addItem("UI_NGP_RAND_AMOUNT_IN_30_TO_100");
    ui->internalAddonAvailabilityCombo->addItem("UI_NGP_RAND_AMOUNT_IN_50_TO_150");
    ui->internalAddonAvailabilityCombo->addItem("UI_NGP_RAND_AMOUNT_IN_0_TO_100");
    ui->internalAddonAvailabilityCombo->addItem("UI_NGP_RANDOM_OPTION");

    ui->vehicleAddonPricingLabel->setText("UI_NGP_ADDON_PRICING");
    ui->vehicleAddonPricingCombo->addItem("UI_NGP_DEFAULT");
    ui->vehicleAddonPricingCombo->addItem("UI_NGP_FREE_ADDONS");
    ui->vehicleAddonPricingCombo->addItem("UI_NGP_X2_PRICES_ADDONS");
    ui->vehicleAddonPricingCombo->addItem("UI_NGP_X4_PRICES_ADDONS");
    ui->vehicleAddonPricingCombo->addItem("UI_NGP_X6_PRICES_ADDONS");
    ui->vehicleAddonPricingCombo->addItem("UI_NGP_RANDOMIZED_X1_TO_X6_ADDONS");
    ui->vehicleAddonPricingCombo->addItem("UI_NGP_RANDOM_OPTION");

    ui->garageRefuelLabel->setText("UI_NGP_GARAGE_REFUEL");
    ui->garageRefuelCombo->addItem("UI_NGP_DEFAULT");
    ui->garageRefuelCombo->addItem("UI_NGP_NO_REFUELNG_IN_GARAGE");
    ui->garageRefuelCombo->addItem("UI_NGP_RANDOM_OPTION");
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
