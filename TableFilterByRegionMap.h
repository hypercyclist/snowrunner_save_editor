#ifndef TABLEFILTERBYREGIONMAP_H
#define TABLEFILTERBYREGIONMAP_H

#include <QWidget>

class GameAtlas;

namespace Ui { class TableFilterByRegionMap; }

class TableFilterByRegionMap : public QWidget
{
    Q_OBJECT
public:
    explicit TableFilterByRegionMap(QWidget *parent = nullptr);
    ~TableFilterByRegionMap();
    Ui::TableFilterByRegionMap* gui();
    void setGameAtlas(GameAtlas* _gameAtlas);

public slots:
    void filterMaps();

private:
    Ui::TableFilterByRegionMap *ui;
    GameAtlas* m_gameAtlas;
};

#endif // TABLEFILTERBYREGIONMAP_H
