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

#ifndef TANO_LISTITEM_H_
#define TANO_LISTITEM_H_

#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtGui/QIcon>

class ListItem : public QObject
{
Q_OBJECT
public:
    ListItem(QObject* parent = 0) : QObject(parent) {}
    virtual ~ListItem() {}
    virtual QString id() const = 0;
    virtual QVariant data(int role) const = 0;
    virtual QString display() const = 0;
    virtual QIcon displayIcon() const = 0;
    virtual QHash<int, QByteArray> roleNames() const = 0;

signals:
    void dataChanged();
};

#endif // TANO_LISTITEM_H_
