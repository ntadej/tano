/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
*
* This file was part of Bitcoin.
* Copyright (C) 2013 Bitcoin Developers
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

#ifndef TANO_NOTIFICATIONS_H_
#define TANO_NOTIFICATIONS_H_

#include <QtCore/QObject>
#include <QtGui/QIcon>

QT_BEGIN_NAMESPACE
class QSystemTrayIcon;
#ifdef Q_OS_LINUX
class QDBusInterface;
#endif
QT_END_NAMESPACE

class Notifications: public QObject
{
Q_OBJECT
public:
    Notifications(QSystemTrayIcon *trayIcon = 0,
                  QObject *parent = 0);
    ~Notifications();

public slots:
    void notify(const QString &title,
                const QString &text,
                int millisTimeout = 5000);

private:
    enum Mode
    {
        None,
        Freedesktop,
        QSystemTray,
        Growl12,
        Growl13,
        OSXNotificationCenter
    };

    Mode _mode;
    QSystemTrayIcon *_trayIcon;
#ifdef Q_OS_LINUX
    QDBusInterface *_interface;

    void notifyDBus(const QString &title,
                    const QString &text,
                    int millisTimeout);
#endif
    void notifySystray(const QString &title,
                       const QString &text,
                       int millisTimeout);
#ifdef Q_OS_MAC
    void notifyGrowl(const QString &title,
                     const QString &text);
    void notifyMacOSXNotificationCenter(const QString &title,
                                        const QString &text);
#endif
};

extern Notifications *notifications;

#endif // TANO_NOTIFICATIONS_H_
