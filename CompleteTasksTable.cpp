#include "CompleteTasksTable.h"
#include <QCheckBox>
#include <Localization.h>
#include <QHBoxLayout>
#include <QHeaderView>
#include <GameAtlas.h>
#include "Region.h"
#include "Map.h"
#include "Task.h"
#include "TableFilterByRegionMap.h"
#include "ui_TableFilterByRegionMap.h"

CompleteTasksTable::CompleteTasksTable(QWidget* _parent) :
    QTableWidget(_parent),
    m_localization(nullptr),
    m_gameAtlas(nullptr),
    m_filterBar(nullptr),
    m_regionFilterCombobox(nullptr),
    m_mapFilterCombobox(nullptr),
    m_filterApplyButton(nullptr),
    m_checkAllFilteredButton(nullptr),
    m_currentFiltredElements()
{ }

void CompleteTasksTable::setLocalization(Localization* _localization)
{
    m_localization = _localization;
}

void CompleteTasksTable::setGameAtlas(GameAtlas* _gameAtlas)
{
    m_gameAtlas = _gameAtlas;
}

void CompleteTasksTable::setFilterBarWidget(TableFilterByRegionMap* _widget)
{
    m_filterBar = _widget;
    m_regionFilterCombobox = m_filterBar->gui()->regionFilterCombobox;
    m_mapFilterCombobox = m_filterBar->gui()->mapFilterCombobox;
    m_filterApplyButton = m_filterBar->gui()->filterApplyButton;
    m_checkAllFilteredButton = m_filterBar->gui()->checkAllFilteredButton;

    connect(m_filterApplyButton, &QPushButton::clicked, this,
        &CompleteTasksTable::updateTable);

    connect(m_checkAllFilteredButton, &QPushButton::clicked, this,
        &CompleteTasksTable::checkUncheckAllFiltered);
}

void CompleteTasksTable::updateTable()
{
    setRowCount(0);
    m_currentFiltredElements.clear();

    QAbstractItemModel* tasksTableModel = model();

    for (auto regionPair : m_gameAtlas->regions())
    {
        std::string regionName = regionPair.second->name(m_localization->defaultLanguage());
        QString currentRegionFilter = m_regionFilterCombobox->currentText();

        if (currentRegionFilter == "Все" || regionName.c_str() == m_regionFilterCombobox->currentText())
        {
            for (auto mapPair : regionPair.second->maps())
            {
                QString  currentMapFilter = m_mapFilterCombobox->currentText();
                if (currentMapFilter == "Все" || mapPair.second->name(m_localization->defaultLanguage()).c_str() == currentMapFilter)
                {
                    for (auto taskPair : mapPair.second->tasks())
                    {
                        int rowIndex = rowCount();
                        insertRow(rowIndex);

                        std::string taskCode = taskPair.first;

                        m_currentFiltredElements.push_back(taskCode.c_str());

                        tasksTableModel->setData(tasksTableModel->index(rowIndex, 0), regionName.c_str());
                        tasksTableModel->setData(tasksTableModel->index(rowIndex, 1), mapPair.second->name(m_localization->defaultLanguage()).c_str());
                        tasksTableModel->setData(tasksTableModel->index(rowIndex, 2), taskPair.second->name(m_localization->defaultLanguage()).c_str());

                        QCheckBox* statusCheckBox = new QCheckBox();
                        statusCheckBox->setChecked(taskPair.second->complete());

                        connect(statusCheckBox, &QCheckBox::stateChanged, this, [=](bool _state)
                        {
                            taskPair.second->setComplete(_state);
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

void CompleteTasksTable::checkUncheckAllFiltered()
{
    // If no one checked - check all. If one checked - uncheck all.
    bool checkedAnyone = false;
    for (int i = 0; i < rowCount(); i++)
    {
        if (static_cast<QCheckBox*>(cellWidget(i, 3)->layout()->itemAt(0)->
            widget())->isChecked())
        {
            checkedAnyone = true;
        }
    }
    for (int i = 0; i < rowCount(); i++)
    {
        static_cast<QCheckBox*>(cellWidget(i, 3)->layout()->itemAt(0)->
            widget())->setChecked(!checkedAnyone);
    }
}
