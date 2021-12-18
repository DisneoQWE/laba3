#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QWidget>
#include <QItemSelection>
#include "Data.h"
#include "FileBrowserModel.h"
#include <memory>

namespace Ui {
    class FileManager;
}
/*В примере используется модель файловой системы (QFileSystemModel)*/
class QFileSystemModel;
class Explorer;

class FileManager : public QWidget
{
    Q_OBJECT

public:
    explicit FileManager(QWidget *parent = nullptr);
    void displayTableModel();
    ~FileManager();
enum class GroupedBy {
    Folders,
    Types
};

protected slots:
    void selectionGroup(int index);
    void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

private:
    Ui::FileManager *ui;
    QFileSystemModel* dirModel;
    Explorer* explorer;
    QList<Data> data;
    QString path;
    GroupedBy grouping;
    std::shared_ptr<FileBrowserModel> fmodel;
};

#endif // FILEMANAGER_H
