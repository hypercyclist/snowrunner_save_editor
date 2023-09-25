#ifndef TASKS_H
#define TASKS_H

#include <QString>
#include <QSet>
#include <QMap>
#include <QVector>

class Localization;

class Tasks
{
public:
    Tasks();
    void loadTasksData();
    QMap<QString, QMap<QString, QVector<QString>>>& tasks();
private:
    // Region, Map, Task - Code.
    QMap<QString, QMap<QString, QVector<QString>>> m_tasks;
};

#endif // TASKS_H
