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

    for (auto regionPair : m_gameAtlas->regions())
    {
        std::string regionName = regionPair.second->name(Language::RUSSIAN);
        QString currentRegionFilter = m_regionFilterCombobox->currentText();

        if (currentRegionFilter == "Все" || regionName.c_str() == m_regionFilterCombobox->currentText())
        {
            for (auto mapPair : regionPair.second->maps())
            {
                QString  currentMapFilter = m_mapFilterCombobox->currentText();
                if (currentMapFilter == "Все" || mapPair.second->name(Language::RUSSIAN).c_str() == currentMapFilter)
                {
                    for (auto upgradePair : mapPair.second->upgrades())
                    {
                        int rowIndex = rowCount();
                        insertRow(rowIndex);

                        std::string upgradeCode = upgradePair.first;

                        m_currentFiltredElements.push_back(upgradeCode.c_str());

                        upgradesTableModel->setData(upgradesTableModel->index(rowIndex, 0), regionName.c_str());
                        upgradesTableModel->setData(upgradesTableModel->index(rowIndex, 1), mapPair.second->name(Language::RUSSIAN).c_str());
//                        upgradesTableModel->setData(upgradesTableModel->index(rowIndex, 2), upgradePair.second->name(Language::RUSSIAN).c_str());
                        upgradesTableModel->setData(upgradesTableModel->index(rowIndex, 2), upgradePair.second->middleCode().c_str());

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
