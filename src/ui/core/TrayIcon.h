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

#ifndef TANO_TRAYICON_H_
#define TANO_TRAYICON_H_

#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QSystemTrayIcon>

#include "core/Enums.h"

class TrayIcon : public QSystemTrayIcon
{
Q_OBJECT
public:
    TrayIcon(QMenu *menu,
             QObject *parent = 0);
    ~TrayIcon();

public slots:
    void message(const Tano::Id &type,
                 const QStringList &arg);
    void changeToolTip(const Tano::Id &type,
                       const QString &text = 0);

signals:
    void restoreClick();

private slots:
    void iconActivated(const QSystemTrayIcon::ActivationReason reason);

private:
    QString _currentlyPlaying;
    QString _currentlyRecording;
};

#endif // TANO_TRAYICON_H_
