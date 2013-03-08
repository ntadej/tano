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

#ifndef TANO_LISTITEM_H_
#define TANO_LISTITEM_H_

#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtGui/QIcon>

/*!
    \class ListItem ListItem.h core/ListItem.h
    \brief List item

    An abstract representation of a list item with basic properties
*/
class ListItem : public QObject
{
Q_OBJECT
public:
    /*!
        \brief Empty constructor
        \param parent parent object (QObject *)
    */
    ListItem(QObject *parent = 0) : QObject(parent) { }
    virtual ~ListItem() { }

    /*!
        \brief Item id
        \return unique item id (QString)
    */
    virtual QString id() const = 0;

    /*!
        \brief Get item data
        \param role selected data role name (int)
        \return data for specific role (QVariant)
    */
    virtual QVariant data(const int &role) const = 0;

    /*!
        \brief Convenience function for Qt::DisplayRole
        \return item display text (QString)
    */
    virtual QString display() const = 0;

    /*!
        \brief Convenience function for Qt::DecorationRole
        \return item decoration/icon (QIcon)
    */
    virtual QIcon decoration() const = 0;

    /*!
        \brief Supported item's role names
        \return item's supported role names (QHash<int, QByteArray>)
    */
    virtual QHash<int, QByteArray> roleNames() const = 0;

signals:
    /*!
        \brief Signal sent on data change
    */
    void dataChanged();
};

#endif // TANO_LISTITEM_H_
