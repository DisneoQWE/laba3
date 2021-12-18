#include "SortFolders.h"
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QTextStream>


qint64 getCurDirSize(const QString& path)
{
    qint64 totalSize = 0;
    QDir dir(path);
    for (const auto& it : dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot , QDir::Name | QDir::Type))
        totalSize += it.size();
    return totalSize;
}

QMap<QString, double> SortFolders::getFoldersPercentOfTotal(qint64& totalSize, QMap<QString, qint64>& FoldersList) const
{
	QMap<QString, double> foldersListPercentage;
    double percent;
	for (auto it = FoldersList.begin(); it != FoldersList.end(); ++it) {
        if (it.value() == 0) {
            percent = 0.0;
        }
        else
        {
            percent = double(it.value() * 100) / totalSize;
            // метка для слишком маленьких папок
            if (percent < 0.01)
                percent = -percent;
        }
        foldersListPercentage.insert(it.key(), percent);

	}
	return foldersListPercentage;
}

QList<QPair<double, QString>> sortByPercent(const QMap<QString, double>& FoldersAndPercentage)
{
    QList<QPair<double, QString>> sortedMap;
    for (auto x : FoldersAndPercentage.keys()) {
      sortedMap.append(QPair<double, QString>(FoldersAndPercentage[x], x));
    }
    // Пропускаем корневую папку
    std::sort(sortedMap.begin() + 1, sortedMap.end(), std::greater<QPair<double, QString>>());;
    return sortedMap;
}

QMap<QString, qint64> SortFolders::getFoldersSizes(const QString& path) const
{
    QFileInfo folder(path);
    // Создаем QMap содержащий полный путь папки и её размер
    QMap<QString, qint64> FoldersList;
    // Вычисляем размер текущей папки
    auto absoluteCurDirPath = folder.absoluteFilePath();
	FoldersList.insert(absoluteCurDirPath, getCurDirSize(absoluteCurDirPath));
    // Вычисляем размеры вложенных папок
    for (auto& x : QDir(path).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden, QDir::Name | QDir::Type))
	{
		auto absolutePath = x.absoluteFilePath();
        FoldersList.insert(absolutePath, Common::getTotalSize(absolutePath));
	}
    return FoldersList;
}

void SortFolders::PrintFoldersSizesAndPercentage(const QMap<QString, qint64>& FoldersAndTypes, const QList<QPair<double, QString> >& FoldersAndPercentage) const
{
    QTextStream out(stdout);
    for (auto&& x : FoldersAndPercentage) {
            out << qSetFieldWidth(45) << x.second << qSetFieldWidth(10)  << FoldersAndTypes.value(x.second) / 1024.0
                      << qSetFieldWidth(4)<< "KB";
            if (x.first < 0) {
                out << qSetFieldWidth(8) << "< 0.01 %\n";
            } else
                out << qSetFieldWidth(8) << QString::number(x.first, 'f', 2).append(" %") << "\n";
    }
}

void SortFolders::explore(const QString& path)
{
    QFileInfo folder(path);
    if (!folder.exists() && !folder.isReadable()){
        qDebug() << "Error! Folder doesn't exist or it's symlink" << Qt::endl;
        return;
    }
    auto FoldersList = getFoldersSizes(path);
    auto totalSize = Common::sumSizes(FoldersList);
    auto FoldersPercentage = getFoldersPercentOfTotal(totalSize, FoldersList);
    auto sortedFoldersPercentage = sortByPercent(FoldersPercentage);
    PrintFoldersSizesAndPercentage(FoldersList, sortedFoldersPercentage);
}

