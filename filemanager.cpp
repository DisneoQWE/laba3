#include "filemanager.h"
#include "ui_filemanager.h"
#include <QFileSystemModel>
#include "SortTypes.h"
#include "SortFolders.h"
#include "FileBrowserModel.h"
#include "ListViewMediator.h"
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

    observers.push_back(new ListViewMediator(ui->stackedWidget->layout()));
    observers.push_back(new PieChart(ui->stackedWidget->layout()));
    observers.push_back(new BarChart(ui->stackedWidget->layout()));

    for (auto& x : observers) {
        FolderGrouping->Attach(x);
        TypesGrouping->Attach(x);
    }
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

    qDeleteAll(observers);
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
    groupingStrategy->explore(path);
}

void FileManager::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);

    QModelIndexList indexes = selected.indexes();
    path = dirModel->filePath(indexes[0]);
    groupingStrategy->explore(path);

}
