#include "EditorWindow.h"
#include "ui_EditorWindow.h"

EditorWindow::EditorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EditorWindow)
{
    ui->setupUi(this);
}

EditorWindow::~EditorWindow()
{
    delete ui;
}

