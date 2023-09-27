#include "CompleteTasksTable.h"
#include <QCheckBox>
#include <Localization.h>
#include <QHBoxLayout>
#include <QHeaderView>

CompleteTasksTable::CompleteTasksTable(QWidget* _parent) :
    QTableWidget(_parent),
    m_localization(nullptr),
    m_tasks(nullptr),
    m_regionFilterCombobox(nullptr),
    m_mapFilterCombobox(nullptr),
    m_filterApplyButton(nullptr),
    m_checkAllFilteredButton(nullptr),
    m_currentFiltredTasks(),
    m_checkedTasks()
{ }

void CompleteTasksTable::setLocalization(Localization* _localization)
{
    m_localization = _localization;
}

void CompleteTasksTable::setTasks(Tasks* _tasks)
{
    m_tasks = _tasks;
}

void CompleteTasksTable::setRegionFilterCombobox(QComboBox* _combobox)
{
    // unbind old.
    m_regionFilterCombobox = _combobox;
    connect(m_regionFilterCombobox, static_cast<void (QComboBox::*)(int)>
        (&QComboBox::currentIndexChanged), this, &CompleteTasksTable::filterMaps);
}

void CompleteTasksTable::setMapFilterCombobox(QComboBox* _combobox)
{
    m_mapFilterCombobox = _combobox;
}

void CompleteTasksTable::setFilterApplyButton(QPushButton* _button)
{
    //unbind old.
    m_filterApplyButton = _button;
    connect(m_filterApplyButton, &QPushButton::clicked, this,
        &CompleteTasksTable::updateTasksTable);
}

void CompleteTasksTable::setCheckAllFilteredButton(QPushButton* _button)
{
    m_checkAllFilteredButton = _button;
    connect(m_checkAllFilteredButton, &QPushButton::clicked, this,
        &CompleteTasksTable::checkUncheckAllFiltered);
}

void CompleteTasksTable::filterMaps()
{
//    if (m_regionFilterCombobox->findText("Все") == -1)
//        m_regionFilterCombobox->addItem("Все");

//    m_mapFilterCombobox->clear();

//    if (m_mapFilterCombobox->findText("Все") == -1)
//        m_mapFilterCombobox->addItem("Все");

//    QString currentRegionFilter = m_regionFilterCombobox->currentText();

//    QMap<QString, QMap<QString, QVector<QString>>>& tasks = m_tasks->tasks();
//    auto regionIt = tasks.begin();
//    while(regionIt != tasks.end())
//    {
//        QString region = m_localization->getLocalization(regionIt.key());
//        region = cutLongCountryName(region);

//        if (m_regionFilterCombobox->findText(region) == -1)
//            m_regionFilterCombobox->addItem(region);

//        if (currentRegionFilter == "Все" || region == currentRegionFilter)
//        {
//            QMap<QString, QVector<QString>>& maps = regionIt.value();
//            auto mapsIt = maps.begin();
//            while (mapsIt != maps.end())
//            {
//                QString mapCode = mapsIt.key();
//                QString map = m_localization->getLocalization(mapCode + "_NAME");
//                if (map == "---")
//                    map = m_localization->getLocalization(mapCode + "_NEW_NAME");
//                if (map == "---")
//                    map = m_localization->getLocalization("LEVEL_" + mapCode + "_NAME");
//                if (map == "---")
//                    map = m_localization->getLocalization("LEVEL_" + mapCode);

//                map = cutSlash(map);
//                m_mapFilterCombobox->addItem(map);
//                mapsIt++;
//            }
//        }
//        regionIt++;
//    }
}

void CompleteTasksTable::updateTasksTable()
{
//    setRowCount(0);
//    m_currentFiltredTasks.clear();

//    QAbstractItemModel* tasksTableModel = model();

//    QMap<QString, QMap<QString, QVector<QString>>>& tasks = m_tasks->tasks();

//    auto regionIt = tasks.begin();
//    while(regionIt != tasks.end())
//    {
//        QString region = m_localization->getLocalization(regionIt.key());
//        region = cutLongCountryName(region);

//        QString currentRegionFilter = m_regionFilterCombobox->currentText();
//        if (currentRegionFilter == "Все" || region == m_regionFilterCombobox->currentText())
//        {
//            QMap<QString, QVector<QString>>& maps = regionIt.value();
//            auto mapsIt = maps.begin();
//            while (mapsIt != maps.end())
//            {
//                QString mapCode = mapsIt.key();
//                QString map = m_localization->getLocalization(mapCode + "_NAME");
//                if (map == "---")
//                    map = m_localization->getLocalization(mapCode + "_NEW_NAME");
//                if (map == "---")
//                    map = m_localization->getLocalization("LEVEL_" + mapCode + "_NAME");
//                if (map == "---")
//                    map = m_localization->getLocalization("LEVEL_" + mapCode);

//                map = cutSlash(map);

//                QString  currentMapFilter = m_mapFilterCombobox->currentText();
//                if (currentMapFilter == "Все" || map == currentMapFilter)
//                {
//                    for (const QString& taskCode : mapsIt.value())
//                    {
//                        int rowIndex = rowCount();
//                        insertRow(rowIndex);

//                        m_currentFiltredTasks.push_back(taskCode);

//                        QString task = m_localization->getLocalization(taskCode);

//                        if (taskCode.right(4) == "_OBJ")
//                        {
//                            task += " (Контракт)";
//                        }

//                        tasksTableModel->setData(tasksTableModel->index(rowIndex, 0), region);
//                        tasksTableModel->setData(tasksTableModel->index(rowIndex, 1), map);
//                        tasksTableModel->setData(tasksTableModel->index(rowIndex, 2), task);

//                        QCheckBox* statusCheckBox = new QCheckBox();
//                        if (m_checkedTasks.contains(taskCode))
//                        {
//                            statusCheckBox->setChecked(m_checkedTasks[taskCode]);
//                        }
//                        connect(statusCheckBox, &QCheckBox::stateChanged, this, [=](bool _state)
//                        {
//                            m_checkedTasks.insert(taskCode, _state);
//                        });

//                        QWidget* statusContainer = new QWidget();
//                        QHBoxLayout* statusContainerLayout = new QHBoxLayout();
//                        statusContainerLayout->setContentsMargins(0, 0, 0, 0);
//                        statusContainerLayout->setAlignment( Qt::AlignCenter );
//                        statusContainerLayout->addWidget(statusCheckBox);
//                        statusContainer->setLayout(statusContainerLayout);
//                        setCellWidget(rowIndex, 3, statusContainer);
//                    }
//                }
//                mapsIt++;
//            }
//        }
//        regionIt++;
//    }
//    horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::ResizeToContents);
//    horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
//    horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeMode::Stretch);
//    horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeMode::Fixed);
//    horizontalHeader()->resizeSection(3, 100);
}

void CompleteTasksTable::setCompleteFromVector(QVector<QString> _completedTasks)
{
    for (const QString& taskCode : _completedTasks)
    {
        m_checkedTasks.insert(taskCode, true);
    }
}

QVector<QString> CompleteTasksTable::completedTasks()
{
    QVector<QString> completedTasks;
    auto tasksIt = m_checkedTasks.begin();
    while (tasksIt != m_checkedTasks.end())
    {
        if (tasksIt.value())
        {
            completedTasks.push_back(tasksIt.key());
        }
        tasksIt++;
    }
    return completedTasks;
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
    for (int i = 0; i < rowCount(); i++)
    {
        QWidget* statusContainer = cellWidget(i, 3);
        m_checkedTasks.insert(m_currentFiltredTasks[i],
            static_cast<QCheckBox*>(statusContainer->layout()->itemAt(0)->
                widget())->isChecked());
    }
}
