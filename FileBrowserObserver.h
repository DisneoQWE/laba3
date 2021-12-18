#ifndef FILEBROWSEROBSERVER_H
#define FILEBROWSEROBSERVER_H
#include <memory>
#include "Data.h"

class FileBrowserObserver
{
public:
    FileBrowserObserver() = default;
    virtual void UpdateDisplay(const std::unique_ptr<QList<Data>>& data) const = 0;
    virtual ~FileBrowserObserver() = default;
};


#endif // FILEBROWSEROBSERVER_H
