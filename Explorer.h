#ifndef EXPLORER_H
#define EXPLORER_H
#include <QDir>
#include <QFileInfo>
#include <QTextStream>


class ExploreInterface {
public:
    virtual void explore(const QString& path) = 0;
    virtual ~ExploreInterface() {}
};

class Explorer
{
private:
    ExploreInterface *p;
public:
    explicit Explorer(ExploreInterface* l) : p(l) {}
    void explore(const QString& path) {
        p->explore(path);
    }
    void setStrategy(ExploreInterface* strategy) { p = strategy; }
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
