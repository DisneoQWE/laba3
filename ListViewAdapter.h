#ifndef LISTVIEWADAPTER_H
#define LISTVIEWADAPTER_H

#include "FileBrowserObserver.h"
#include "FileBrowserModel.h"
#include <QTableView>

class ListViewAdapter : public FileBrowserObserver
{
private:
    FileBrowserModel* model;
    QTableView* view;
public:
    explicit ListViewAdapter(QLayout* l);
    ~ListViewAdapter();

    void UpdateDisplay(const std::unique_ptr<QList<Data> > &data) const override;
};



#endif // LISTVIEWADAPTER_H
