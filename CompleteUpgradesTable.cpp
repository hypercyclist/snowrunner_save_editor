#include "CompleteUpgradesTable.h"
#include <QCheckBox>
#include <Localization.h>
#include <QHBoxLayout>
#include <QHeaderView>
#include <GameAtlas.h>
#include "Region.h"
#include "Map.h"
#include "Upgrade.h"

CompleteUpgradesTable::CompleteUpgradesTable(QWidget* _parent) :
    CompleteTasksTable(_parent)
{ }

void CompleteUpgradesTable::updateTable()
{
    setRowCount(0);
    m_currentFiltredElements.clear();

    QAbstractItemModel* upgradesTableModel = model();

    std::string allString = m_localization->localization("UI_DLC_STORE_ALL_DLCS", m_localization->defaultLanguage());

    for (auto regionPair : m_gameAtlas->regions())
    {
        std::string regionName = regionPair.second->name(m_localization->defaultLanguage());
        QString currentRegionFilter = m_regionFilterCombobox->currentText();

        if (currentRegionFilter == allString.c_str() || regionName.c_str() == m_regionFilterCombobox->currentText())
        {
            for (const auto& mapPair : regionPair.second->maps())
            {
                QString  currentMapFilter = m_mapFilterCombobox->currentText();
                if (currentMapFilter == allString.c_str() || mapPair.second->name(m_localization->defaultLanguage()).c_str() == currentMapFilter)
                {
                    for (const auto& upgradePair : mapPair.second->upgrades())
                    {
                        int rowIndex = rowCount();
                        insertRow(rowIndex);

                        std::string upgradeCode = upgradePair.first;

                        m_currentFiltredElements.push_back(upgradeCode.c_str());

                        upgradesTableModel->setData(upgradesTableModel->index(rowIndex, 0), regionName.c_str());
                        upgradesTableModel->setData(upgradesTableModel->index(rowIndex, 1), mapPair.second->name(m_localization->defaultLanguage()).c_str());

                        std::string trucksList;
                        for (std::string truckCode : upgradePair.second->trucks())
                        {
                            if (trucksList.size() > 0) trucksList += ", ";
                            trucksList += m_localization->localization(truckCode, m_localization->defaultLanguage());
                        }

                        std::string upgradeName = upgradeTypeToText(upgradePair.second, m_localization->defaultLanguage())
                            + ": " + upgradePair.second->name(m_localization->defaultLanguage()).c_str() + " (" + trucksList + ")";

                        upgradesTableModel->setData(upgradesTableModel->index(rowIndex, 2), upgradeName.c_str());

                        QCheckBox* statusCheckBox = new QCheckBox();
                        statusCheckBox->setChecked(upgradePair.second->received());

                        connect(statusCheckBox, &QCheckBox::stateChanged, this, [=](bool _state)
                        {
                            upgradePair.second->setReceived(_state);
                        });

                        QWidget* statusContainer = new QWidget();
                        QHBoxLayout* statusContainerLayout = new QHBoxLayout();
                        statusContainerLayout->setContentsMargins(0, 0, 0, 0);
                        statusContainerLayout->setAlignment( Qt::AlignCenter );
                        statusContainerLayout->addWidget(statusCheckBox);
                        statusContainer->setLayout(statusContainerLayout);
                        setCellWidget(rowIndex, 3, statusContainer);
                    }
                }
            }
        }
    }

    horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeMode::Stretch);
    horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeMode::Fixed);
    horizontalHeader()->resizeSection(3, 100);
}

std::string CompleteUpgradesTable::upgradeTypeToText(Upgrade* upgrade,
    Language _language)
{
    UpgradeType upgradeType = upgrade->type();
    switch (upgradeType)
    {
    case UpgradeType::ENGINE: return m_localization->localization("UI_TRUCK_PART_ENGINE", _language); break;
    case UpgradeType::GEARBOX: return m_localization->localization("UI_TRUCK_PART_GEARBOX", _language); break;
    case UpgradeType::SUSPENSION: return m_localization->localization("UI_TRUCK_PART_SUSPENSION", _language); break;
    case UpgradeType::DIFFLOCK: return m_localization->localization("UI_TRUCK_INFO_DIFF_LOCK", _language); break;
    case UpgradeType::TRANSFERBOX: return m_localization->localization("UI_UPGRADE_TRANSFERBOX_AWDSWITCHABLE_NAME", _language); break;
    default: return "Unknown"; break;
    }
}
