#ifndef NEWGAMEPLUSSETTINGSTABLE_H
#define NEWGAMEPLUSSETTINGSTABLE_H

#include <QWidget>

class Localization;
class SaveFile;

namespace Ui { class NewGamePlusSettingsTable; }

struct NewGamePlusSettings
{
    int trailerPricingFactor =  2;
    bool isGarageRefuelAvailable =  true;
    int autoloadPrice =  0;
    std::vector<int> recoveryPrice;// =  null;
    int truckAvailability =  0;
    int truckAvailabilityLevel =  10;
    int startingLevelState =  0;
    int trailerAvailability =  0;
    std::vector<int> fullRepairPrice;// =  null;
    float sellMultiplier =  0.0;
    int regionTravellingPriceFactor =  2;
    int addonPricingFactor =  2;
    float addonSellingFactor =  0.5;
    int RandomSeed =  0;
    float timeSettingsDay =  0.0;
    bool needToAddDlcTrucks =  false;
    std::vector<int> overrideStartingProfileData;// =  null;
    int startingTrucks =  0;
    int maxContestAttempts =  0;
    std::vector<int> deployPrice;// =  null;
    std::vector<int> zonePropsToDisable;// =  null;
    std::vector<int> startingGarageStatus;// =  null;
    std::vector<int> objectivesToComplete;// =  null;
    int internalAddonAvailability =  0;
    bool recoverOnStart =  false;
    bool isGoldFailReason =  false;
    std::string startingTruck =  "";
    int ranNumToDisableTrailerStore =  1233;
    int truckPricingFactor =  2;
    int vehicleStorageSlots =  0;
    bool isMapMarkerAsInHardMode =  false;
    bool isDLCVehiclesAvailable =  false;
    int internalAddonAmount =  0;
    int trailerStoreAviability =  0;
    float truckSellingFactor =  1.0;
    int externalAddonsAmount =  0;
    bool isNeedToRandomGarageRepaireFactorInEveryZone =  false;
    float trailerSellingFactor =  1.0;
    int tyreAvailability =  0;
    int externalAddonAvailability =  0;
    int fuelPriceFactor =  2;
    int garageRepairePriceFactor =  1;
    int loadingPriceFactor =  1;
    int repairPointsCostFactor =  3;
    float repairPointsRequiredFactor =  0.5;
    float contractsPayoutsFactor =  2.0;
    float tasksAndContestsPayoutsFactor =  2.0;
    int regionRepairePointsFactor =  1;
    int regionRepaireMoneyFactor =  1;
    int vehicleDamageFactor =  1;
    int recoveryPriceFactor =  -1;
    float timeSettingsNight =  0.0;
    bool isAbleToSkipTime =  true;
    int teleportationPrice =  1000;
    std::vector<int> settingsDictionaryForNGPScreen;// =  null
};

class NewGamePlusSettingsTable : public QWidget
{
    Q_OBJECT

public:
    explicit NewGamePlusSettingsTable(QWidget *parent = nullptr);
    ~NewGamePlusSettingsTable();

    void setLocalization(Localization* _localization);
//    void setNewGamePlusSettings(NewGamePlusSettings* _newGamePlusSettings);
    void setSaveFile(SaveFile* _saveFile);

    void updateTable();

private:
    Ui::NewGamePlusSettingsTable *ui;
    Localization* m_localization;
    NewGamePlusSettings m_newGamePlusSettings;
    SaveFile* m_saveFile;
};

#endif // NEWGAMEPLUSSETTINGSTABLE_H
