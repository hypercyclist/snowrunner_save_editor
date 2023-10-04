#ifndef TABLEFILTERBYREGIONMAP_H
#define TABLEFILTERBYREGIONMAP_H

#include <QWidget>

class Localization;
class GameAtlas;

namespace Ui { class TableFilterByRegionMap; }

class TableFilterByRegionMap : public QWidget
{
    Q_OBJECT
public:
    explicit TableFilterByRegionMap(QWidget *parent = nullptr);
    ~TableFilterByRegionMap();
    Ui::TableFilterByRegionMap* gui();
    void setLocalization(Localization* _localization);
    void setGameAtlas(GameAtlas* _gameAtlas);

public slots:
    void filterMaps();

private:
    Ui::TableFilterByRegionMap *ui;
    Localization* m_localization;
    GameAtlas* m_gameAtlas;
};

#endif // TABLEFILTERBYREGIONMAP_H
