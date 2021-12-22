#include "ListViewMediator.h"
#include "Explorer.h"
#include <QLayout>
#include <QHeaderView>

ListViewMediator::ListViewMediator(QLayout* l)
{
    model = new FileBrowserModel();
    view = new QTableView();
    view->setModel(model);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    l->addWidget(view);
}

void ListViewMediator::UpdateDisplay(const QList<Data> &data)
{
    model->setModelData(data);
}

ListViewMediator::~ListViewMediator()
{
    delete model;
    delete view;
}

QList<Data> ListViewMediator::data() const
{
    return model->getData();
}
