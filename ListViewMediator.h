#ifndef LISTVIEWMEDIATOR_H
#define LISTVIEWMEDIATOR_H

#include "FileBrowserObserver.h"
#include "FileBrowserModel.h"
#include <QTableView>

class ListViewMediator : public FileBrowserObserver
{
private:
    FileBrowserModel* model;
    QTableView* view;
public:
    explicit ListViewMediator(QLayout* l);
    ~ListViewMediator();
    QList<Data> data() const override;
    void UpdateDisplay(const QList<Data> &data) override;
};



#endif // LISTVIEWMEDIATOR_H
