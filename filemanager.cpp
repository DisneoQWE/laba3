#include "filemanager.h"
#include "ui_filemanager.h"
#include <QFileSystemModel>
#include "SortTypes.h"
#include "SortFolders.h"
#include "FileBrowserModel.h"
#include "ListViewAdapter.h"
#include "Charts.h"

#include <QDebug>

FileManager::FileManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileManager),
    FolderGrouping(new SortFolders()),
    TypesGrouping(new SortTypes()),
    groupingStrategy(FolderGrouping)
{
    ui->setupUi(this);
    dirModel = new QFileSystemModel(this);

    list_view_adapter = new ListViewAdapter(ui->stackedWidget->layout());
    pie_chart = new PieChart(ui->stackedWidget->layout());
    bar_chart = new BarChart(ui->stackedWidget->layout());

    FileBrowserView = list_view_adapter;
    groupingStrategy->Attach(FileBrowserView);
    this->setMinimumSize(1200, 500);
    dirModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Hidden);
    dirModel->setRootPath(QDir::currentPath());
    ui->treeView->setModel(dirModel);
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    connect(ui->displayBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FileManager::selectionDisplay);
    connect(ui->groupBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FileManager::selectionGroup);
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this,  &FileManager::selectionChanged);
}


FileManager::~FileManager()
{
    delete ui;
    delete dirModel;

    // очищаем память из под адаптеров
    delete list_view_adapter;
    delete pie_chart;
    delete bar_chart;

    delete FolderGrouping;
    delete TypesGrouping;
}


void FileManager::displayTableModel()
{
    if (path.isEmpty())
        return;

}



void FileManager::selectionDisplay(int index)
{
    switch(index)
    {
        case 0:
            FileBrowserView = list_view_adapter;
            break;
        case 1:
            FileBrowserView = pie_chart;
            break;
        case 2:
            FileBrowserView = bar_chart;
            break;
    }
    groupingStrategy->Attach(FileBrowserView);
    groupingStrategy->explore(path);
    ui->stackedWidget->setCurrentIndex(index);
}

void FileManager::selectionGroup(int index)
{
    switch (index) {
        case 0:
            groupingStrategy = FolderGrouping;
            break;
        case 1:
            groupingStrategy = TypesGrouping;
            break;
        default:
            std::exit(-1);
            break;
    }
    groupingStrategy->Attach(FileBrowserView);
    groupingStrategy->explore(path);
}

void FileManager::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);

    QModelIndexList indexes = selected.indexes();
    path = dirModel->filePath(indexes[0]);
    groupingStrategy->explore(path);
}
