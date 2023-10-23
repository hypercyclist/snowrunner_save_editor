#include "TableFilterByRegionMap.h"
#include "ui_TableFilterByRegionMap.h"
#include "GameAtlas.h"
#include "Region.h"
#include "Map.h"
#include "Localization.h"

TableFilterByRegionMap::TableFilterByRegionMap(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableFilterByRegionMap),
    m_localization(nullptr),
    m_gameAtlas(nullptr)
{
    ui->setupUi(this);

    connect(ui->regionFilterCombobox, static_cast<void (QComboBox::*)(int)>
        (&QComboBox::currentIndexChanged), this, &TableFilterByRegionMap::filterMaps);
}

TableFilterByRegionMap::~TableFilterByRegionMap()
{
    delete ui;
}

void TableFilterByRegionMap::setLocalization(Localization* _localization)
{
    m_localization = _localization;
}

Ui::TableFilterByRegionMap* TableFilterByRegionMap::gui()
{
    return ui;
}

void TableFilterByRegionMap::setGameAtlas(GameAtlas* _gameAtlas)
{
    m_gameAtlas = _gameAtlas;
}

void TableFilterByRegionMap::filterMaps()
{
//    ui->regionFilterCombobox->blockSignals(true);
    disconnect(ui->regionFilterCombobox, static_cast<void (QComboBox::*)(int)>
        (&QComboBox::currentIndexChanged), this, &TableFilterByRegionMap::filterMaps);

    std::string currentRegion = ui->regionFilterCombobox->currentText().toStdString();
    std::string currentMap = ui->mapFilterCombobox->currentText().toStdString();

    ui->regionFilterCombobox->clear();
    ui->mapFilterCombobox->clear();

    std::string allString = m_localization->localization("UI_DLC_STORE_ALL_DLCS", m_localization->defaultLanguage());

    if (ui->regionFilterCombobox->findText(allString.c_str()) == -1)
        ui->regionFilterCombobox->addItem(allString.c_str());

    if (ui->mapFilterCombobox->findText(allString.c_str()) == -1)
        ui->mapFilterCombobox->addItem(allString.c_str());


    for (const auto& regionPair : m_gameAtlas->regions())
    {
        std::string regionName = regionPair.second->name(m_localization->defaultLanguage());
        if (ui->regionFilterCombobox->findText(regionName.c_str()) == -1)
            ui->regionFilterCombobox->addItem(regionName.c_str());

        if (currentRegion == allString.c_str() || currentRegion == regionName.c_str())
        {
            for (const auto& mapPair : regionPair.second->maps())
            {
                ui->mapFilterCombobox->addItem(mapPair.second->name(m_localization->defaultLanguage()).c_str());
            }
        }
    }

    ui->regionFilterCombobox->setCurrentText(currentRegion.c_str());

    connect(ui->regionFilterCombobox, static_cast<void (QComboBox::*)(int)>
        (&QComboBox::currentIndexChanged), this, &TableFilterByRegionMap::filterMaps);
//    ui->regionFilterCombobox->blockSignals(false);
}
