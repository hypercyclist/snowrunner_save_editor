#include "InformationWidget.h"
#include "ui_InformationWidget.h"

InformationWidget::InformationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InformationWidget)
{
    ui->setupUi(this);
}

InformationWidget::~InformationWidget()
{
    delete ui;
}
