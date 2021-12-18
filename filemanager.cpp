#include "filemanager.h"
#include "ui_filemanager.h"
#include <QFileSystemModel>
#include "Explorer.h"
#include "SortTypes.h"
#include "SortFolders.h"
#include "FileBrowserModel.h"
#include <QDebug>

FileManager::FileManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileManager)
{
    ui->setupUi(this);
    dirModel = new QFileSystemModel(this);
    grouping = GroupedBy::Folders;
    fmodel = std::make_shared<FileBrowserModel>();
    explorer = new Explorer();

    this->setMinimumSize(1200, 500);
    dirModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Hidden);
    dirModel->setRootPath(QDir::currentPath());
    ui->treeView->setModel(dirModel);
    ui->tableView->setModel(fmodel.get());
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    connect(ui->groupBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FileManager::selectionGroup);
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this,  &FileManager::selectionChanged);
}

void FileManager::displayTableModel()
{
    if (path.isEmpty())
        return;
    switch (grouping) {
    case GroupedBy::Folders:
        explorer->setStrategy(new SortFolders);
        break;
    case GroupedBy::Types:
        explorer->setStrategy(new SortTypes);
        break;
    default:
        explorer->setStrategy(new SortFolders);
        break;
    }
    data = explorer->explore(path);
    fmodel->setModelData(data);
}

FileManager::~FileManager()
{
    delete ui;
    delete dirModel;
    delete explorer;
}

void FileManager::selectionGroup(int index)
{
    switch (index) {
        case 0:
            grouping = GroupedBy::Folders;
            break;
        case 1:
            grouping = GroupedBy::Types;
            break;
        default:
            grouping = GroupedBy::Folders;
            break;
    }
    displayTableModel();
}

void FileManager::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);

    QModelIndexList indexes = selected.indexes();
    path = dirModel->filePath(indexes[0]);
    displayTableModel();
}
