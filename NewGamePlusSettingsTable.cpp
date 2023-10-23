#include "NewGamePlusSettingsTable.h"
#include "ui_NewGamePlusSettingsTable.h"
#include "Localization.h"
//#include "NewGamePlusSettings.h"

NewGamePlusSettingsTable::NewGamePlusSettingsTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewGamePlusSettingsTable),
    m_localization(nullptr)
{
    ui->setupUi(this);
}

NewGamePlusSettingsTable::~NewGamePlusSettingsTable()
{
    delete ui;
}

void NewGamePlusSettingsTable::setLocalization(Localization* _localization)
{
    m_localization = _localization;
}

//void NewGamePlusSettingsTable::setNewGamePlusSettings(NewGamePlusSettings* _newGamePlusSettings)
//{
//    m_newGamePlusSettings = _newGamePlusSettings;
//}

void NewGamePlusSettingsTable::setSaveFile(SaveFile* _saveFile)
{
    m_saveFile = _saveFile;

}

void NewGamePlusSettingsTable::updateTable()
{
    ui->truckAvailabilityLabel->setText(m_localization->localization("UI_TRUCK_AVAILABILITY").c_str());
    QComboBox* combo = ui->truckAvailabilityCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_ALL_TRUCKS_FROM_START").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_TRUCKS_IN_EVERY_GARAGE").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_TRUCK_STORE_FROM_LVL_10").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_TRUCK_STORE_FROM_LVL_20").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_TRUCK_STORE_FROM_LVL_30").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_UNAVAILABLE_TRUCK_STORE").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

    int index = 0;
//    NewGamePlusSettings* ngps = m_newGamePlusSettings;
//    if (ngps->truckAvailability == 1)
//    {
//        index = 1;
////        if (...) index = 2;
//    }
//    else
//    {
//        if (ngps->truckAvailabilityLevel == 10) index = 3;
//        else if (ngps->truckAvailabilityLevel == 20) index = 4;
//        else if (ngps->truckAvailabilityLevel == 30) index = 5;
//        else index = 6;
//    }
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->dlcVehiclesAvailabilityLabel->setText(m_localization->localization("UI_DLC_VEHICLES").c_str());
    combo = ui->dlcVehiclesAvailabilityCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_AVAILABLE").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_UNAVAILABLE").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->truckPricingLabel->setText(m_localization->localization("UI_TRUCK_PRICING").c_str());
    combo = ui->truckPricingCombo;
    combo->clear();
    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
    combo->addItem(m_localization->localization("UI_NGP_FREE_TRUCKS").c_str());
    combo->addItem(m_localization->localization("UI_NGP_X2_PRICES_TRUCKS").c_str());
    combo->addItem(m_localization->localization("UI_NGP_X4_PRICES_TRUCKS").c_str());
    combo->addItem(m_localization->localization("UI_NGP_X6_PRICES_TRUCKS").c_str());
    combo->addItem(m_localization->localization("UI_NGP_RANDOMIZED_X1_TO_X6_TRUCKS").c_str());
    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

    index = 0;
//    if (ngps->truckPricingFactor == 1) index = 0;
//    if (ngps->truckPricingFactor == -1) index = 1;
//    if (ngps->truckPricingFactor == 2) index = 2;
//    else if (ngps->truckPricingFactor == 4) index = 3;
//    else if (ngps->truckPricingFactor == 6) index = 4;
//    combo->setCurrentIndex(index);

    connect(combo, static_cast<void (QComboBox::*)(int)>
        (&QComboBox::currentIndexChanged), this, [=](int _index)
    {
//        if (_index == 0)
    });

    ui->truckSellingLabel->setText(m_localization->localization("UI_NGP_TRUCK_SELLING").c_str());
    combo = ui->truckSellingCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_0.5X_PRICES_TRUCKS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_0.3X_PRICES_TRUCKS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_0.1X_PRICES_TRUCKS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_UNABLE_TO_SELL_TRUCKS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->vehicleStorageAvailabilityLabel->setText(m_localization->localization("UI_VEHICLE_STORAGE_AVAILABILITY").c_str());
    combo = ui->vehicleStorageAvailabilityCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_3_SLOTS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_5_SLOTS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_10_SLOTS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_ONLY_SCOUT_VEHICLES").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->trailerAvailabilityLabel->setText(m_localization->localization("UI_NGP_TRALER_AVAILABILITY").c_str());
    combo = ui->trailerAvailabilityCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_ALL_TRAILERS_AVAILABLE").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_ONLY_FOUND_IN_WORLD").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->trailerStoreAvailabilityLabel->setText(m_localization->localization("UI_NGP_TRAILER_STORE_AVAILABILITY").c_str());
    combo = ui->trailerStoreAvailabilityCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_ONLY_FOUND_IN_WORLD").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->trailerPricingLabel->setText(m_localization->localization("UI_NP_TRAILER_PRICING").c_str());
    combo = ui->trailerPricingCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_ALL_TRAILERS_FREE").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X2_PRICES_TRAILERS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X4_PRICES_TRAILERS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X6_PRICES_TRAILERS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOMIZED_X1_TO_X6_TRAILERS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->trailerSellingPriceLabel->setText(m_localization->localization("UI_NGP_TRAILER_SELLING").c_str());
    combo = ui->trailerSellingPriceCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_0.5X_PRICES_TRAILERS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_0.3X_PRICES_TRAILERS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_0.1X_PRICES_TRAILERS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_UNABLE_TO_SELL_TRAILERS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->externalBodyAddonsAvailabilityLabel->setText(m_localization->localization("UI_NGP_ALL_ADDONS_AVAILABLE").c_str());
    combo = ui->externalBodyAddonsAvailabilityCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_ALL_ADDONS_AVAILABLE").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RAND_SET_OF_5_ADDONS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RAND_SET_OF_10_ADDONS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RAND_SET_OF_10_IN_EVERY_GARAGE_ADDONS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->tireAvailAbilityLabel->setText(m_localization->localization("UI_TYRE_AVAILABILITY").c_str());
    combo = ui->tireAvailAbilityCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_ALL_TYRES_AVAILABLE").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_ONLY_HIGHWAY_AND_ALLROAD").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_ONLY_HIGHWAY_AND_ALLROAD_AND_OFFROAD").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_NO_MUD_TYRES").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_NO_CHAINED_TYRES").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_SET_OF_RANDOM_IN_EVETY_GARAGE").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->internalAddonAvailabilityLabel->setText(m_localization->localization("UI_INTERNAL_ADDON_AVAILABILITY").c_str());
    combo = ui->internalAddonAvailabilityCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_ALL_INTERNAL_ADDONS_AVAILABLE").c_str()); // Need to check!
//    combo->addItem(m_localization->localization("UI_NGP_RAND_AMOUNT_IN_10_TO_50").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RAND_AMOUNT_IN_30_TO_100").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RAND_AMOUNT_IN_50_TO_150").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RAND_AMOUNT_IN_0_TO_100").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->vehicleAddonPricingLabel->setText(m_localization->localization("UI_NGP_ADDON_PRICING").c_str());
    combo = ui->vehicleAddonPricingCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_FREE_ADDONS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X2_PRICES_ADDONS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X4_PRICES_ADDONS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X6_PRICES_ADDONS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOMIZED_X1_TO_X6_ADDONS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->garageRefuelLabel->setText(m_localization->localization("UI_NGP_GARAGE_REFUEL").c_str());
    combo = ui->vehicleAddonPricingCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_NO_REFUELNG_IN_GARAGE").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->fuelPricingLabel->setText(m_localization->localization("UI_NGP_FUEL_PRICE").c_str());
    combo = ui->vehicleAddonPricingCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_FREE_FUEL").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X2_PRICES_FUEL").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X4_PRICES_FUEL").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X6_PRICES_FUEL").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOMIZED_X1_TO_X6_FUEL").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_COST_FROM_X1_TO_X6_ON_EVERY_STATION_FUEL").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->garageVehicleRepairLabel->setText(m_localization->localization("UI_NGP_GARAGE_REPAIRE").c_str());
    combo = ui->garageVehicleRepairCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_UNABLE_TO_REPAIRE_AUTOMATICALLY").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X2_PRICES_REPAIRE").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X4_PRICES_REPAIRE").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X6_PRICES_REPAIRE").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_COST_FROM_X1_TO_X6_IN_EVERY_GARAGE_REPAIRE").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->repairePointsRestockPriceLabel->setText(m_localization->localization("UI_NGP_REPAIRE_POINTS_RESTOCK_PRICE").c_str()); // Check.
    combo = ui->repairePointsRestockPriceCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_POINTS_COST_RULES").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_POINTS_COST_X2_PRICES").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_POINTS_COST_X4_PRICES").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_POINTS_COST_X6_PRICES").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_COST_FROM_X1_TO_X6_IN_EVERY_SUPPLY_ZONE_POINTS_COST").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->repairPointsEfficiencyLabel->setText(m_localization->localization("UI_NGP_REPAIR_POINTS_EFFICIENCY").c_str());
    combo = ui->repairPointsEfficiencyCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_HARD_MODE_X0.5_POINTS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_HARD_MODE_X2_POINTS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_HARD_MODE_X4_POINTS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_HARD_MODE_X6_POINTS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->vehicleRepairRegionalLabel->setText(m_localization->localization("UI_NGP_REGIONAL_REPAIR").c_str());
    combo = ui->vehicleRepairRegionalCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X2PRICE_X2POINTS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X3PRICE_X3POINTS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X4PRICE_X4POINTS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->vehicleDamageLabel->setText(m_localization->localization("UI_VEHICLE_DAMAGE").c_str());
    combo = ui->vehicleDamageCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_NO_DAMAGE").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X2POINTS_DAMAGE").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X3POINTS_DAMAGE").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X5POINTS_DAMAGE").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->truckRecoveryLabel->setText(m_localization->localization("UI_NGP_RECOVERY_RULES").c_str());
    combo = ui->truckRecoveryCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X2PRICE_RECOVERY").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X4PRICE_RECOVERY").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X6PRICE_RECOVERY").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_NOT_AVAILABLE_RECOVERY").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->contractPayoutLabel->setText(m_localization->localization("UI_NGP_CONTRACT_PAYOUTS").c_str());
    combo = ui->contractPayoutCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X0.5_PRICES_CONTRACTS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X1.5_PRICES_CONTRACTS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X2_PRICES_CONTRACTS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X3_PRICES_CONTRACTS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOMIZED_FROM_X0.5_TO_X3_CONTRACTS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->tasksAndContestPayoutLabel->setText(m_localization->localization("UI_NGP_TASKS_AND_CONTESTS_PAYOUTS").c_str());
    combo = ui->tasksAndContestPayoutCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X0.5_PRICES_TASKS_CONTESTS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X1.5_PRICES_TASKS_CONTESTS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X2_PRICES_TASKS_CONTESTS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X3_PRICES_TASKS_CONTESTS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOMIZED_FROM_X0.5_TO_X3_TASKS_CONTESTS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->contestAvailabilityLabel->setText(m_localization->localization("UI_NGP_CONTESTS_AVAILABILITY").c_str());
    combo = ui->contestAvailabilityCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_1_ATTEMPT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_3_ATTEMPTS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_5_ATTEMPTS").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_GOLD_TIME").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->automaticCargoLoadingLabel->setText(m_localization->localization("UI_NGP_LOADING_RULES").c_str());
    combo = ui->automaticCargoLoadingCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X1PRICE_LOADING").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X2PRICE_LOADING").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X4PRICE_LOADING").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X6PRICE_LOADING").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->mapMarkerLabel->setText(m_localization->localization("UI_NGP_MAP_MARKER_RULES").c_str());
    combo = ui->mapMarkerCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_HARD_MODE_RULES").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->travellingBetweenRegionsLabel->setText(m_localization->localization("UI_NGP_REGION_TRAVELLING_PRICING").c_str());
    combo = ui->travellingBetweenRegionsCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X2PRICE_TRAVEL").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X4PRICE_TRAVEL").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_X6PRICE_TRAVEL").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->truckSwitchingLabel->setText(m_localization->localization("UI_NGP_TELEPORTATION_RULES").c_str());
    combo = ui->truckSwitchingCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_500_PRICE").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_1000_PRICE").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_2000_PRICE").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_5000_PRICE").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });

    ui->timeSettingsLabel->setText(m_localization->localization("UI_NGP_TIME_SETTINGS").c_str());
    combo = ui->timeSettingsCombo;
    combo->clear();
//    combo->addItem(m_localization->localization("UI_NGP_DEFAULT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_UNABLE_TO_SKIP_TIME").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_DAYX2").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_NIGHTX2").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_ALWAYS_DAY").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_ALWAYS_NIGHT").c_str());
//    combo->addItem(m_localization->localization("UI_NGP_RANDOM_OPTION").c_str());

//    index = 0;
//    combo->setCurrentIndex(index);

//    connect(combo, static_cast<void (QComboBox::*)(int)>
//        (&QComboBox::currentIndexChanged), this, [=](int _index)
//    {

//    });
}
