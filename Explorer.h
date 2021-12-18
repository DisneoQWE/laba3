#ifndef EXPLORER_H
#define EXPLORER_H
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include "Data.h"

class ExploreInterface {
public:
    virtual QList<Data> explore(const QString& path) = 0;
    virtual ~ExploreInterface() {}
};

class Explorer
{
private:
    ExploreInterface *p = nullptr;
public:
    Explorer() = default;
    explicit Explorer(ExploreInterface* l) : p(l) {}
    QList<Data> explore(const QString& path) {
        return p->explore(path);
    }
    void setStrategy(ExploreInterface* strategy) {
        if (p)
            delete p;
        p = strategy;
    }
    ~Explorer() { if (p) delete p; }
};

namespace Common {
    inline qint64 getTotalSize(const QString& path)
    {
        qint64 totalSize = 0;
        QDir dir(path);
        for (const auto& it : dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden, QDir::Name | QDir::Type | QDir::Size))
                if (it.isDir() && !it.isSymLink()) {
                    totalSize += getTotalSize(it.absoluteFilePath());
                } else totalSize += it.size();
        return totalSize;
    }
    inline qint64 sumSizes(const QMap<QString, qint64> Sizes)
    {
        qint64 totalSize = 0;
        foreach(qint64 size, Sizes.values())
            totalSize += size;
        return totalSize;
    }
}

#endif //EXPLORER_H
