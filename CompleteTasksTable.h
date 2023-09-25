#ifndef COMPLETETASKSTABLE_H
#define COMPLETETASKSTABLE_H

#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>

class Localization;
class Tasks;

class CompleteTasksTable : public QTableWidget
{
    Q_OBJECT
public:
    CompleteTasksTable(QWidget* _parent);
    void setLocalization(Localization* _localization);
    void setTasks(Tasks* _tasks);
    void setRegionFilterCombobox(QComboBox* _combobox);
    void setMapFilterCombobox(QComboBox* _combobox);
    void setFilterApplyButton(QPushButton* _button);
    void setCheckAllFilteredButton(QPushButton* _button);
    void filterMaps();
    void updateTasksTable();
    void setCompleteFromVector(QVector<QString> _completedTasks);
    QVector<QString> completedTasks();
private:
    Localization* m_localization;
    Tasks* m_tasks;
    QComboBox* m_regionFilterCombobox;
    QComboBox* m_mapFilterCombobox;
    QPushButton* m_filterApplyButton;
    QPushButton* m_checkAllFilteredButton;

    QVector<QString> m_currentFiltredTasks;
    QMap<QString, bool> m_checkedTasks;
private slots:
    void checkUncheckAllFiltered();
};

#endif // COMPLETETASKSTABLE_H
