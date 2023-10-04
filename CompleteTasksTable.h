#ifndef COMPLETETASKSTABLE_H
#define COMPLETETASKSTABLE_H

#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>

class Localization;
class GameAtlas;
class TableFilterByRegionMap;

class CompleteTasksTable : public QTableWidget
{
    Q_OBJECT
public:
    CompleteTasksTable(QWidget* _parent);
    void setLocalization(Localization* _localization);
    void setGameAtlas(GameAtlas* _gameAtlas);
    void setFilterBarWidget(TableFilterByRegionMap* _widget);

    virtual void updateTable();
    QVector<QString> checkedElements();
protected:
    Localization* m_localization;
    GameAtlas* m_gameAtlas;

    TableFilterByRegionMap* m_filterBar;
    QComboBox* m_regionFilterCombobox;
    QComboBox* m_mapFilterCombobox;
    QPushButton* m_filterApplyButton;
    QPushButton* m_checkAllFilteredButton;
    QVector<QString> m_currentFiltredElements;

private slots:
    void checkUncheckAllFiltered();
};

#endif // COMPLETETASKSTABLE_H
