/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_XMLTVCREWMEMBER_H_
#define TANO_XMLTVCREWMEMBER_H_

#include <QtCore/QList>
#include <QtCore/QString>

#include "core/ListItem.h"
#include "xmltv/XmltvEnums.h"

class XmltvCrewMember : public ListItem
{
Q_OBJECT
public:
    enum Roles {
        DisplayRole = Qt::DisplayRole,
        DisplayIconRole = Qt::DecorationRole,
        NameRole = Qt::UserRole + 1,
        TypeRole
    };

    XmltvCrewMember(const QString &name = 0,
                    const Tano::Xmltv::CrewMemberType &type = Tano::Xmltv::Actor,
                    QObject *parent = 0);
    ~XmltvCrewMember();

    inline QString id() const { return _name; }
    QVariant data(int role) const;
    QString display() const;
    QIcon displayIcon() const;
    QHash<int, QByteArray> roleNames() const;

    inline QString name() const { return _name; }
    inline Tano::Xmltv::CrewMemberType type() const { return _type; }

private:
    QString _name;
    Tano::Xmltv::CrewMemberType _type;
};

#endif // TANO_XMLTVCREWMEMBER_H_
