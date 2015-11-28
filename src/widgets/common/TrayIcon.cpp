/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
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

#include "common/Common.h"

#include "TrayIcon.h"

TrayIcon::TrayIcon(QMenu *menu,
                   QObject *parent)
    : QSystemTrayIcon(parent)
{
    setContextMenu(menu);
    setIcon(QIcon(":/logo/48x48/logo.png"));
    setToolTip(Tano::name());

    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

TrayIcon::~TrayIcon() { }

void TrayIcon::iconActivated(const QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        emit restoreClick();
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        break;
    }
}
