#ifndef SORTFOLDERS_H
#define SORTFOLDERS_H
#include "Explorer.h"


class SortFolders final : public ExploreInterface
{
public:
    explicit SortFolders() = default;
    virtual ~SortFolders() {}
    void explore(const QString& path) override;
private:
    QMap<QString, qint64> getFoldersSizes(const QString& path) const;
    QMap<QString, double> getFoldersPercentOfTotal(qint64& totalSize, QMap<QString, qint64>& FoldersList) const;
    void PrintFoldersSizesAndPercentage(const QMap<QString, qint64>& FoldersAndTypes, const QList<QPair<double, QString> >& FoldersAndPercentage) const;
    QList<Data> CombineData(const QMap<QString, qint64>& FoldersAndTypes, const QList<QPair<double, QString>>& FoldersAndPercentage) const;
};

#endif //SORTFOLDERS_H
