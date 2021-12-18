#ifndef SORTTYPES_H
#define SORTTYPES_H
#include "Explorer.h"

class SortTypes : public ExploreInterface {
public:
    explicit SortTypes() = default;
    virtual ~SortTypes() = default;
    void explore(const QString& path) override;
private:
    void getFileTypesAndSizes(const QString& path, QMap<QString, qint64>& FileTypesList);
    QMap<QString, double> getFileTypesPercentOfTotal(qint64& totalSize, QMap<QString, qint64>& FileTypesList) const;
    void PrintFileTypesListAndPercents(const QMap<QString, qint64>& FileTypesList, const QList<QPair<double, QString>> FileTypesPercantage) const;
    QList<QPair<double, QString>> sortByPercent(const QMap<QString, double>& FileTypesPercantage);
};


#endif // SORTTYPES_H
