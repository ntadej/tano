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

#include <QtCore/QTemporaryFile>
#include <QtGui/QImageWriter>

#if QT_VERSION >= 0x050000
    #include <QtWidgets/QApplication>
    #include <QtWidgets/QSystemTrayIcon>
    #include <QtWidgets/QStyle>
#else
    #include <QtGui/QApplication>
    #include <QtGui/QSystemTrayIcon>
    #include <QtGui/QStyle>
#endif

#ifdef Q_OS_LINUX
    #include <QtDBus/QDBusInterface>
    #include "common/FreedesktopImage.h"
#endif

#ifdef Q_OS_MAC
    #include <ApplicationServices/ApplicationServices.h>
    #include "application/NotificationsOSX.h"
#endif

#include "core/Common.h"

#include "Notifications.h"

Notifications *notifications = 0;

Notifications::Notifications(QSystemTrayIcon *trayicon,
                             QObject *parent)
    : QObject(parent),
      _mode(None),
      _trayIcon(trayicon)
#ifdef Q_OS_LINUX
      , _interface(0)
#endif
{
    if(trayicon && trayicon->supportsMessages()) {
        _mode = QSystemTray;
    }
#ifdef Q_OS_LINUX
    _interface = new QDBusInterface("org.freedesktop.Notifications",
          "/org/freedesktop/Notifications", "org.freedesktop.Notifications");
    if(_interface->isValid()) {
        _mode = Freedesktop;
    }
#endif
#ifdef Q_OS_MAC
    // check if users OS has support for NSUserNotification
    if (NotificationsOSX::instance()->hasNotificationCenterSupport()) {
        _mode = OSXNotificationCenter;
        NotificationsOSX::instance()->init();
    } else {
        // Check if Growl is installed (based on Qt's tray icon implementation)
        CFURLRef cfurl;
        OSStatus status = LSGetApplicationForInfo(kLSUnknownType, kLSUnknownCreator, CFSTR("growlTicket"), kLSRolesAll, 0, &cfurl);
        if (status != kLSApplicationNotFoundErr) {
            CFBundleRef bundle = CFBundleCreate(0, cfurl);
            if (CFStringCompare(CFBundleGetIdentifier(bundle), CFSTR("com.Growl.GrowlHelperApp"), kCFCompareCaseInsensitive | kCFCompareBackwards) == kCFCompareEqualTo) {
                if (CFStringHasSuffix(CFURLGetString(cfurl), CFSTR("/Growl.app/")))
                    _mode = Growl13;
                else
                    _mode = Growl12;
            }
            CFRelease(cfurl);
            CFRelease(bundle);
        }
    }
#endif
}

Notifications::~Notifications()
{
#ifdef Q_OS_LINUX
    delete _interface;
#endif
}

#ifdef Q_OS_LINUX
void Notifications::notifyDBus(const QString &title,
                               const QString &text,
                               int millisTimeout)
{
    // Arguments for DBus call:
    QList<QVariant> args;

    // Program Name:
    args.append(Tano::name());

    // Unique ID of this notification type:
    args.append(0U);

    // Application Icon, empty string
    args.append(QString());

    // Summary
    args.append(title);

    // Body
    args.append(text);

    // Actions (none, actions are deprecated)
    QStringList actions;
    args.append(actions);

    // Hints
    QVariantMap hints;

    // If no icon specified, set icon based on class
    QIcon tmpicon = QIcon(":/logo/128x128/logo.png");
    hints["icon_data"] = FreedesktopImage::toVariant(tmpicon.pixmap(FREEDESKTOP_NOTIFICATION_ICON_SIZE).toImage());
    args.append(hints);

    // Timeout (in msec)
    args.append(millisTimeout);

    // "Fire and forget"
    _interface->callWithArgumentList(QDBus::NoBlock, "Notify", args);
}
#endif

void Notifications::notifySystray(const QString &title,
                                  const QString &text,
                                  int millisTimeout)
{
    _trayIcon->showMessage(title, text, QSystemTrayIcon::Information, millisTimeout);
}

// Based on Qt's tray icon implementation
#ifdef Q_OS_MAC
void Notifications::notifyGrowl(const QString &title,
                                const QString &text)
{
    const QString script(
        "tell application \"%5\"\n"
        "  set the allNotificationsList to {\"Notification\"}\n" // -- Make a list of all the notification types (all)
        "  set the enabledNotificationsList to {\"Notification\"}\n" // -- Make a list of the notifications (enabled)
        "  register as application \"%1\" all notifications allNotificationsList default notifications enabledNotificationsList\n" // -- Register our script with Growl
        "  notify with name \"Notification\" title \"%2\" description \"%3\" application name \"%1\"%4\n" // -- Send a Notification
        "end tell"
    );

    QString notificationApp(QApplication::applicationName());
    if (notificationApp.isEmpty())
        notificationApp = "Application";

    QPixmap notificationIconPixmap = QIcon(":/logo/48x48/logo.png").pixmap(48);
    QString notificationIcon;
    QTemporaryFile notificationIconFile;
    if (!notificationIconPixmap.isNull() && notificationIconFile.open()) {
        QImageWriter writer(&notificationIconFile, "PNG");
        if (writer.write(notificationIconPixmap.toImage()))
            notificationIcon = QString(" image from location \"file://%1\"").arg(notificationIconFile.fileName());
    }

    QString quotedTitle(title), quotedText(text);
    quotedTitle.replace("\\", "\\\\").replace("\"", "\\");
    quotedText.replace("\\", "\\\\").replace("\"", "\\");
    QString growlApp(this->_mode == Notifications::Growl13 ? "Growl" : "GrowlHelperApp");
    NotificationsOSX::instance()->sendAppleScript(script.arg(notificationApp, quotedTitle, quotedText, notificationIcon, growlApp));
}

void Notifications::notifyMacOSXNotificationCenter(const QString &title,
                                                   const QString &text)
{
    NotificationsOSX::instance()->showNotification(title, text);
}
#endif

void Notifications::notify(const QString &title,
                           const QString &text,
                           int millisTimeout)
{
    switch(_mode)
    {
#ifdef Q_OS_LINUX
    case Freedesktop:
        notifyDBus(title, text, millisTimeout);
        break;
#endif
    case QSystemTray:
        notifySystray(title, text, millisTimeout);
        break;
#ifdef Q_OS_MAC
    case OSXNotificationCenter:
        notifyMacOSXNotificationCenter(title, text);
        break;
    case Growl12:
    case Growl13:
        notifyGrowl(title, text);
        break;
#endif
    default:
        break;
    }
}
