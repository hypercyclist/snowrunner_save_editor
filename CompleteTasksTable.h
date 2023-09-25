#ifndef COMPLETETASKSTABLE_H
#define COMPLETETASKSTABLE_H

#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>

class CompleteTasksTable : public QTableWidget
{
public:
    CompleteTasksTable();
    void setRegionFilterCombobox(QComboBox* _combobox);
    void setMapFilterCombobox(QComboBox* _combobox);
    void setFilterApplyButton(QPushButton* _button);
    void filterMaps();
    void updateTasksTable();
private:
    QComboBox* m_regionFilterCombobox;
    QComboBox* m_mapFilterCombobox;
    QPushButton* m_filterApplyButton;
};

#endif // COMPLETETASKSTABLE_H
