#include "SortTypes.h"
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>


void SortTypes::getFileTypesAndSizes(const QString& path, QMap<QString, qint64>& FileTypesList)
{
    QDir dir(path);
    for (const auto& it : dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden, QDir::Name | QDir::Type | QDir::Size)) {
            if (it.isDir() && !it.isSymLink()) {
                getFileTypesAndSizes(it.absoluteFilePath(), FileTypesList);
            } else  {
                if (FileTypesList.keys().contains(it.suffix().toLower())) {
                    FileTypesList[it.suffix()] += it.size();
                }
                else {
                    FileTypesList.insert(it.suffix().toLower(), it.size());
                }
            }
    }
}

QMap<QString, double> SortTypes::getFileTypesPercentOfTotal(qint64& totalSize, QMap<QString, qint64>& FileTypesList) const
{
    QMap<QString, double> FileTypesListPercentage;
    double percent;
    for (auto it = FileTypesList.begin(); it != FileTypesList.end(); ++it) {
        if (it.value() == 0) {
            percent = 0.0;
        }
        else {
            percent = double(it.value() * 100) / totalSize;
            if (percent < 0.01)
                percent = -percent;
        }
        FileTypesListPercentage.insert(it.key(), percent);
    }
    return FileTypesListPercentage;
}

QList<QPair<double, QString>> SortTypes::sortByPercent(const QMap<QString, double>& FileTypesPercantage) {
    QList<QPair<double, QString>> sortedMap;
    for (auto x : FileTypesPercantage.keys()) {
      sortedMap.append(QPair<double, QString>(FileTypesPercantage[x], x));
    }
    std::sort(sortedMap.begin(), sortedMap.end(), std::greater<QPair<double, QString>>());
    return sortedMap;
}

void SortTypes::PrintFileTypesListAndPercents(const QMap<QString, qint64>& FileTypesList, const QList<QPair<double, QString>> FileTypesPercantage) const
{
    QTextStream out(stdout);
    if (FileTypesPercantage.isEmpty()) {
        return;
    }
    for (auto&& x : FileTypesPercantage) {
        out << qSetFieldWidth(15) <<   "*." + x.second <<
                    qSetFieldWidth(10) << FileTypesList.value(x.second) / 1024.0 <<
                    qSetFieldWidth(3) << "KB";
                    if (x.first < 0) {
                        out << qSetFieldWidth(8) << "< 0.01 %\n";
                    } else
                        out << qSetFieldWidth(8) << QString::number(x.first, 'f', 2).append(" %") << "\n";
    }
    out.reset();
}


void SortTypes::explore(const QString& path)
{
    QDir folder(path);
    if (!folder.exists() && !folder.isReadable()) {
        qDebug() << "Your path doesn't exist... as your life purpose" << Qt::endl;
        return;
    }
    if (folder.isEmpty()) {
        qDebug() << "There is a lot of space in your folder so IT'S EMPTY!" << Qt::endl;
        return;
    }
    QMap<QString, qint64> fileTypesList;
    getFileTypesAndSizes(path, fileTypesList);
    auto totalSize = Common::sumSizes(fileTypesList);
    auto fileTypesPercantage = getFileTypesPercentOfTotal(totalSize, fileTypesList);
    auto sortedFileTypesPercantage = sortByPercent(fileTypesPercantage);
    PrintFileTypesListAndPercents(fileTypesList, sortedFileTypesPercantage);
}

