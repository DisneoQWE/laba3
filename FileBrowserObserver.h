#ifndef FILEBROWSEROBSERVER_H
#define FILEBROWSEROBSERVER_H
#include <memory>
#include "Data.h"

class FileBrowserObserver
{
public:
    FileBrowserObserver() = default;
    virtual void UpdateDisplay(const QList<Data>& data) = 0;
    virtual ~FileBrowserObserver() = default;
};


#endif // FILEBROWSEROBSERVER_H
