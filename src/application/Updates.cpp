/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2015 Tadej Novak <tadej@tano.si>
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

#include "application/Updates.h"

#if defined(Q_OS_MAC)
    #include <Foundation/NSString.h>
    #include <Sparkle/Sparkle.h>
#elif defined(Q_OS_WIN)
    #include "winsparkle.h"
#endif

Updates::Updates(QObject *parent)
    : QObject(parent)
{
#if defined(Q_OS_MAC)
    [SUUpdater sharedUpdater];
#elif defined(Q_OS_WIN)
    win_sparkle_init();
#endif
}

Updates::~Updates()
{
#if defined(Q_OS_WIN)
    win_sparkle_cleanup();
#endif
}

void Updates::checkForUpdates()
{
#if defined(Q_OS_MAC)
    [[SUUpdater sharedUpdater] checkForUpdates:0];
#elif defined(Q_OS_WIN)
    win_sparkle_check_update_with_ui();
#endif
}
