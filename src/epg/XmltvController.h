/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_XMLTVCONTROLLER_H_
#define TANO_XMLTVCONTROLLER_H_

#include <QtDBus/QDBusAbstractInterface>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusPendingReply>

class XmltvController : public QDBusAbstractInterface
{
Q_OBJECT
public:
    XmltvController(QObject *parent = 0);
    ~XmltvController();

    static const char *staticInterfaceName() { return "si.Tano.Xmltv"; }

public slots:
    QStringList grabbers();

    void refreshSettings();
};

#endif // TANO_XMLTVCONTROLLER_H_
