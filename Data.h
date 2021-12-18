#ifndef DATA_H
#define DATA_H
#include <QString>

struct Data
{
    QString _name;
    QString _size;
    QString _percent;
    Data(const QString& name, const QString& size, const QString& percent)
                : _name(name), _size(size), _percent(percent) {}
};

#endif
