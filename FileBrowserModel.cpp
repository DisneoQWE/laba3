#include "FileBrowserModel.h"
#include <QLocale>

FileBrowserModel::FileBrowserModel(const QList<Data> &data, QObject *parent) : QAbstractTableModel(parent)
{
    _data = data;
}

int FileBrowserModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _data.count();
}


int FileBrowserModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return int(ColumnName::PERCENT) + 1;
}

  QVariant FileBrowserModel::data(const QModelIndex& index, int role) const
  {
      if (!index.isValid() || _data.count() <= index.row() || (role != Qt::DisplayRole && role != Qt::EditRole)) {
          return QVariant();
      }
      switch (index.column()) {
        case 0: return _data[index.row()]._name;
        case 1:
        {
            QLocale locale(QLocale::English);
            return locale.formattedDataSize(_data[index.row()]._size.toDouble());
        }
        case 2: return _data[index.row()]._percent;
        default: return QVariant();
      }
  }

  QVariant FileBrowserModel::headerData(int section, Qt::Orientation orientation, int role) const
  {
      if (role != Qt::DisplayRole) {
          return QVariant();
      }
      if (orientation == Qt::Vertical) {
          return section;
      }
      switch (section) {
      case int(ColumnName::NAME):
            return QString::fromUtf8("Название");
      case int(ColumnName::SIZE):
            return QString::fromUtf8("Размер");
      case int(ColumnName::PERCENT):
            return QString::fromUtf8("В процентах");
      }
      return QVariant();
  }

  void FileBrowserModel::setModelData(const QList<Data> &data)
  {
      beginResetModel();
       _data = data;
      endResetModel();
  }

