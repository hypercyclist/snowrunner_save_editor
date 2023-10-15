#include "EditorWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EditorWindow saveEditor;
    saveEditor.setWindowTitle("SnowRunner - Save editor");
    saveEditor.setWindowIcon(QIcon(":/images/logo.png"));
    saveEditor.show();
    return a.exec();
}
