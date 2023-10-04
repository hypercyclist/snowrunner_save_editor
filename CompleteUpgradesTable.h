#ifndef COMPLETEUPGRADESTABLE_H
#define COMPLETEUPGRADESTABLE_H

#include "CompleteTasksTable.h"

class CompleteUpgradesTable : public CompleteTasksTable
{
public:
    CompleteUpgradesTable(QWidget* _parent);
    void updateTable();
};

#endif // COMPLETEUPGRADESTABLE_H
