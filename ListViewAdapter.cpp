#include "ListViewAdapter.h"
#include "Explorer.h"
#include <QLayout>
#include <QHeaderView>

ListViewAdapter::ListViewAdapter(QLayout* l)
{
    model = new FileBrowserModel();
    view = new QTableView();
    view->setModel(model);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    l->addWidget(view);
}

void ListViewAdapter::UpdateDisplay(const std::unique_ptr<QList<Data> > &data) const
{
    model->setModelData(*data);
}

ListViewAdapter::~ListViewAdapter()
{
    delete model;
    delete view;
}
