#include "Tasks.h"
#include <QFile>
#include <QSet>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "Localization.h"
#include <QDir>

Tasks::Tasks() :
    m_tasks()
{ }

void Tasks::loadTasksData()
{
    QFile tasksFile(QDir::currentPath() + "/jsons/tasks.json");
    tasksFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString tasksText = tasksFile.readAll();
    tasksFile.close();
    rapidjson::Document tasksjsonDocument;
    tasksjsonDocument.Parse(tasksText.toUtf8());

    rapidjson::Value& tasksArray = tasksjsonDocument["Tasks"].GetArray();

    for (rapidjson::SizeType i = 0; i < tasksArray.Size(); i++)
    {
        QString taskCode = tasksArray[i]["task_code"].GetString();
        QString region = taskCode.left(5);
        QString map = taskCode.left(8);

        if (!m_tasks.contains(region))
        {
            QMap<QString, QVector<QString>> regionMap;
            m_tasks.insert(region, regionMap);
        }
        if (!m_tasks[region].contains(map))
        {
            QVector<QString> mapMap;
            m_tasks[region].insert(map, mapMap);
        }
        m_tasks[region][map].push_back(taskCode);
    }
}

QMap<QString, QMap<QString, QVector<QString>>>& Tasks::tasks()
{
    return m_tasks;
}
