#ifndef EXPLORER_H
#define EXPLORER_H
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include "Data.h"
#include "FileBrowserObserver.h"

class ExploreInterface {
public:
    virtual void explore(const QString& path) = 0;
    virtual ~ExploreInterface() { }

    // привязка наблюдателя
    void Attach(FileBrowserObserver* observer) {
        if (observer)
              observer_.push_back(observer);
    }
    void Detach(FileBrowserObserver* observer) {
        if (observer)
            observer_.removeOne(observer);
    }
    // событие окончания формирования данных
    void OnFinish(const QList<Data>& data) const {
        for (auto& x : observer_)
            x->UpdateDisplay(data);
    }
    void OverSub(FileBrowserObserver* observer) {
        if (observer) {
            observer_.removeOne(observer);
            observer_.push_back(observer);
        }
    }
private:
    QList<FileBrowserObserver*> observer_;
};

class Explorer
{
private:
    ExploreInterface *p = nullptr;
public:
    Explorer() = default;
    explicit Explorer(ExploreInterface* l) : p(l) {}
    void explore(const QString& path) {
        p->explore(path);
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
