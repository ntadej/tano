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

#ifndef TANO_DBUSADAPTORXMLTV_H_
#define TANO_DBUSADAPTORXMLTV_H_

#include <QtCore/QObject>
#include <QtDBus/QtDBus>

#include "daemon/xmltv/XmltvCore.h"

class DaemonMain;

class DBusAdaptorXmltv : public QDBusAbstractAdaptor
{
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "si.Tano.Xmltv")

public:
    DBusAdaptorXmltv(XmltvCore *xmltv, DaemonMain *daemon);
    ~DBusAdaptorXmltv();

public slots:
    QStringList grabbers() { return _core->grabbers(); }
    void refreshSettings() { return _core->settings(); }

private:
    XmltvCore *_core;
};

#endif // TANO_DBUSADAPTORXMLTV_H_
