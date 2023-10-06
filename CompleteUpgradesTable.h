#ifndef COMPLETEUPGRADESTABLE_H
#define COMPLETEUPGRADESTABLE_H

#include "CompleteTasksTable.h"

class Upgrade;
enum class Language;

class CompleteUpgradesTable : public CompleteTasksTable
{
public:
    CompleteUpgradesTable(QWidget* _parent);
    void updateTable();
private:
    std::string upgradeTypeToText(Upgrade* upgrade, Language _language);
};

#endif // COMPLETEUPGRADESTABLE_H
