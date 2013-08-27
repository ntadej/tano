/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
*
* This file was part of Bitcoin
* Copyright (C) 2013 Bitcoin Developers.
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

#include "application/NotificationsOSX.h"

#undef slots

#include <Cocoa/Cocoa.h>

void NotificationsOSX::showNotification(const QString &title,
                                        const QString &text)
{
    // check if users OS has support for NSUserNotification
    if (this->hasNotificationCenterSupport()) {
        // okay, seems like 10.8+
        QByteArray utf8 = title.toUtf8();
        char* cString = (char *)utf8.constData();
        NSString *titleMac = [[NSString alloc] initWithUTF8String:cString];

        utf8 = text.toUtf8();
        cString = (char *)utf8.constData();
        NSString *textMac = [[NSString alloc] initWithUTF8String:cString];

        // do everything weak linked (because we will keep <10.8 compatibility)
        id userNotification = [[NSClassFromString(@"NSUserNotification") alloc] init];
        [userNotification performSelector:@selector(setTitle:) withObject:titleMac];
        [userNotification performSelector:@selector(setInformativeText:) withObject:textMac];

        id notificationCenterInstance = [NSClassFromString(@"NSUserNotificationCenter") performSelector:@selector(defaultUserNotificationCenter)];
        [notificationCenterInstance performSelector:@selector(deliverNotification:) withObject:userNotification];

        [titleMac release];
        [textMac release];
        [userNotification release];
    }
}

// sendAppleScript just take a QString and executes it as apple script
void NotificationsOSX::sendAppleScript(const QString &script)
{
    QByteArray utf8 = script.toUtf8();
    char* cString = (char *)utf8.constData();
    NSString *scriptApple = [[NSString alloc] initWithUTF8String:cString];

    NSAppleScript *as = [[NSAppleScript alloc] initWithSource:scriptApple];
    NSDictionary *err = nil;
    [as executeAndReturnError:&err];
    [as release];
    [scriptApple release];
}

bool NotificationsOSX::hasNotificationCenterSupport(void)
{
    Class possibleClass = NSClassFromString(@"NSUserNotificationCenter");

    // check if users OS has support for NSUserNotification
    if (possibleClass != nil) {
        return true;
    }
    return false;
}

NotificationsOSX *NotificationsOSX::instance()
{
    static NotificationsOSX *s_instance = NULL;
    if (!s_instance)
        s_instance = new NotificationsOSX();
    return s_instance;
}
