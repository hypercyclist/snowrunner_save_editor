#include "TableFilterByRegionMap.h"
#include "ui_TableFilterByRegionMap.h"
#include "GameAtlas.h"
#include "Region.h"
#include "Map.h"
#include "Localization.h"

TableFilterByRegionMap::TableFilterByRegionMap(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableFilterByRegionMap),
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
    if (ui->regionFilterCombobox->findText("Все") == -1)
        ui->regionFilterCombobox->addItem("Все");

    ui->mapFilterCombobox->clear();

    if (ui->mapFilterCombobox->findText("Все") == -1)
        ui->mapFilterCombobox->addItem("Все");

    QString currentRegionFilter = ui->regionFilterCombobox->currentText();

    for (auto regionPair : m_gameAtlas->regions())
    {
        std::string regionName = regionPair.second->name(Language::RUSSIAN);
        if (ui->regionFilterCombobox->findText(regionName.c_str()) == -1)
            ui->regionFilterCombobox->addItem(regionName.c_str());

        if (currentRegionFilter == "Все" || regionName.c_str() == currentRegionFilter)
        {
            for (auto mapPair : regionPair.second->maps())
            {
                ui->mapFilterCombobox->addItem(mapPair.second->name(Language::RUSSIAN).c_str());
            }
        }
    }
}
