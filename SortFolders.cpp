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
        if (totalSize != 0) {
            percent = double(it.value() * 100) / totalSize;
            // метка для слишком маленьких папок
            if (percent < 0.01 && percent != 0)
                percent = -percent;
        } else {
            percent = 0;
        }
        foldersListPercentage.insert(it.key(), percent);
	}
	return foldersListPercentage;
}

QList<QPair<double, QString>> sortByPercent(const QMap<QString, double>& FoldersAndPercentage)
{
    QList<QPair<double, QString>> sortedMap;
    for (const auto& x : FoldersAndPercentage.keys()) {
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
            out.setFieldAlignment(QTextStream::AlignLeft);
            out << qSetFieldWidth(45) << x.second <<
            qSetFieldWidth(10)  << FoldersAndTypes.value(x.second) / 1024.0 <<
            qSetFieldWidth(3) << "KB";
            if (x.first < 0) {
                out << qSetFieldWidth(7) << "< 0.01 %" << qSetFieldWidth(0) << endl;
            } else
                out << qSetFieldWidth(7) << QString::number(x.first, 'f', 2).append(" %") << qSetFieldWidth(0) << endl;
    }
    out.reset();
}

QList<Data> SortFolders::CombineData(const QMap<QString, qint64> &FoldersAndTypes, const QList<QPair<double, QString> > &FoldersAndPercentage) const
{
    QList<Data> data;
    auto totalSize = Common::sumSizes(FoldersAndTypes);
    for (auto&& x : FoldersAndPercentage) {
        if (x.first < 0) {
            data.push_back(Data(x.second, QString::number(FoldersAndTypes.value(x.second)), QString("< 0.01 %"), (qreal)FoldersAndTypes.value(x.second)/ totalSize));
        } else {
            data.push_back(Data(x.second, QString::number(FoldersAndTypes.value(x.second)), QString::number(x.first, 'f', 2).append(" %"),(qreal)FoldersAndTypes.value(x.second)/ totalSize));
        }
    }
    return data;
}

void SortFolders::explore(const QString& path)
{
    QFileInfo folder(path);
    if (!folder.exists() && !folder.isReadable()){
        qDebug() << "Error! Folder doesn't exist or it's symlink" << endl;
        return;// empty data or return error
    }
    auto FoldersList = getFoldersSizes(path);
    auto totalSize = Common::sumSizes(FoldersList);
    auto FoldersPercentage = getFoldersPercentOfTotal(totalSize, FoldersList);
    auto sortedFoldersPercentage = sortByPercent(FoldersPercentage);
//    PrintFoldersSizesAndPercentage(FoldersList, sortedFoldersPercentage);

    auto data = CombineData(FoldersList, sortedFoldersPercentage);
    OnFinish(QList<Data>(data));
}

