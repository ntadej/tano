/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
* Based on ListModel by Christophe Dumez <dchris@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef TANO_LISTMODEL_H_
#define TANO_LISTMODEL_H_

#include <QtCore/QAbstractListModel>
#include <QtCore/QList>
#include <QtCore/QVariant>

#include "CoreSharedExport.h"

class ListItem;

/*!
    \class ListModel
    \brief List model

    An abstract representation of a list model with basic properties
*/
class TANO_CORE_EXPORT ListModel : public QAbstractListModel
{
Q_OBJECT
public:
    ListModel(ListItem *prototype, QObject *parent = 0);
    ~ListModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;
    void appendRow(ListItem *item);
    void appendRows(const QList<ListItem *> &items);
    void insertRow(const int &row, ListItem *item);
    bool moveRow(const int &oldRow, const int &newRow, const QModelIndex &parent = QModelIndex());
    bool removeRow(const int &row, const QModelIndex &parent = QModelIndex());
    bool removeRows(const int &row, const int &count, const QModelIndex &parent = QModelIndex());
    ListItem *row(const int &row);
    ListItem *takeRow(const int &row);
    ListItem *find(const QString &id) const;
    QModelIndex indexFromItem(const ListItem *item) const;
    void clear();

private slots:
    void handleItemChange();

private:
    ListItem *_prototype;
    QList<ListItem *> _list;
};

#endif // TANO_LISTMODEL_H_
