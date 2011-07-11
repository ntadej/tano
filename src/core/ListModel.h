/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
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

class ListItem: public QObject
{
Q_OBJECT
public:
    ListItem(QObject* parent = 0) : QObject(parent) {}
    virtual ~ListItem() {}
    virtual QString id() const = 0;
    virtual QVariant data(int role) const = 0;
    virtual QHash<int, QByteArray> roleNames() const = 0;

signals:
    void dataChanged();
};

class ListModel : public QAbstractListModel
{
Q_OBJECT
public:
    ListModel(ListItem *prototype, QObject *parent = 0);
    ~ListModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void appendRow(ListItem *item);
    void appendRows(const QList<ListItem *> &items);
    void insertRow(const int &row, ListItem *item);
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
