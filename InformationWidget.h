#ifndef INFORMATIONWIDGET_H
#define INFORMATIONWIDGET_H

#include <QWidget>

namespace Ui {
class InformationWidget;
}

class InformationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InformationWidget(QWidget *parent = nullptr);
    ~InformationWidget();

private:
    Ui::InformationWidget *ui;
};

#endif // INFORMATIONWIDGET_H
