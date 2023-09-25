#include "CompleteTasksTable.h"
#include <QCheckBox>

CompleteTasksTable::CompleteTasksTable() :
    m_regionFilterCombobox(nullptr),
    m_mapFilterCombobox(nullptr),
    m_filterApplyButton(nullptr)
{ }

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

QString cutLongCountryName(QString _countryName)
{
    if (_countryName.contains("Российская Федерация", Qt::CaseInsensitive))
    {
        _countryName.replace("Российская Федерация", "РФ", Qt::CaseInsensitive);
    }
    return _countryName;
}

QString cutSlash(QString _string)
{
    if (_string.contains("\\"))
    {
        _string.remove("\\");
    }
    return _string;
}

void CompleteTasksTable::filterMaps()
{
    if (m_regionFilterCombobox->findText("Все") == -1)
        m_regionFilterCombobox->addItem("Все");

    m_mapFilterCombobox->clear();

    if (m_mapFilterCombobox->findText("Все") == -1)
        m_mapFilterCombobox->addItem("Все");

    QString currentRegionFilter = m_regionFilterCombobox->currentText();

    QMap<QString, QMap<QString, QVector<QString>>>& tasks = m_tasks->tasks();
    auto regionIt = tasks.begin();
    while(regionIt != tasks.end())
    {
        QString region = m_localization->getLocalization(regionIt.key());
        region = cutLongCountryName(region);

        if (m_regionFilterCombobox->findText(region) == -1)
            m_regionFilterCombobox->addItem(region);

        if (currentRegionFilter == "Все" || region == currentRegionFilter)
        {
            QMap<QString, QVector<QString>>& maps = regionIt.value();
            auto mapsIt = maps.begin();
            while (mapsIt != maps.end())
            {
                QString mapCode = mapsIt.key();
                QString map = m_localization->getLocalization(mapCode + "_NAME");
                if (map == "---")
                    map = m_localization->getLocalization(mapCode + "_NEW_NAME");
                if (map == "---")
                    map = m_localization->getLocalization("LEVEL_" + mapCode + "_NAME");
                if (map == "---")
                    map = m_localization->getLocalization("LEVEL_" + mapCode);

                map = cutSlash(map);
                m_mapFilterCombobox->addItem(map);
                mapsIt++;
            }
        }
        regionIt++;
    }
}

void CompleteTasksTable::updateTasksTable()
{
    ui->completeTasksTable->setRowCount(0);

    QAbstractItemModel* tasksTableModel = ui->completeTasksTable->model();

    QMap<QString, QMap<QString, QVector<QString>>>& tasks = m_tasks->tasks();

    auto regionIt = tasks.begin();
    while(regionIt != tasks.end())
    {
        QString region = m_localization->getLocalization(regionIt.key());
        region = cutLongCountryName(region);

        QString currentRegionFilter = m_regionFilterCombobox->currentText();
        if (currentRegionFilter == "Все" || region == m_regionFilterCombobox->currentText())
        {
            QMap<QString, QVector<QString>>& maps = regionIt.value();
            auto mapsIt = maps.begin();
            while (mapsIt != maps.end())
            {
                QString mapCode = mapsIt.key();
                QString map = m_localization->getLocalization(mapCode + "_NAME");
                if (map == "---")
                    map = m_localization->getLocalization(mapCode + "_NEW_NAME");
                if (map == "---")
                    map = m_localization->getLocalization("LEVEL_" + mapCode + "_NAME");
                if (map == "---")
                    map = m_localization->getLocalization("LEVEL_" + mapCode);

                map = cutSlash(map);

                QString  currentMapFilter = m_mapFilterCombobox->currentText();
                if (currentMapFilter == "Все" || map == currentMapFilter)
                {
                    for (QString taskCode : mapsIt.value())
                    {
                        int rowIndex = ui->completeTasksTable->rowCount();
                        ui->completeTasksTable->insertRow(rowIndex);

                        QString task = m_localization->getLocalization(taskCode);

                        if (taskCode.right(4) == "_OBJ")
                        {
                            task += " (Контракт)";
                        }

                        tasksTableModel->setData(tasksTableModel->index(rowIndex, 0), region);
                        tasksTableModel->setData(tasksTableModel->index(rowIndex, 1), map);
                        tasksTableModel->setData(tasksTableModel->index(rowIndex, 2), task);

                        QCheckBox* statusCheckBox = new QCheckBox();
                        QWidget* statusContainer = new QWidget();
                        QHBoxLayout* statusContainerLayout = new QHBoxLayout();
                        statusContainerLayout->setContentsMargins(0, 0, 0, 0);
                        statusContainerLayout->setAlignment( Qt::AlignCenter );
                        statusContainerLayout->addWidget(statusCheckBox);
                        statusContainer->setLayout(statusContainerLayout);
                        ui->completeTasksTable->setCellWidget(rowIndex, 3, statusContainer);
                    }
                }
                mapsIt++;
            }
        }
        regionIt++;
    }
    ui->completeTasksTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::ResizeToContents);
    ui->completeTasksTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
    ui->completeTasksTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeMode::Stretch);
    ui->completeTasksTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeMode::Fixed);
    ui->completeTasksTable->horizontalHeader()->resizeSection(3, 100);
}
