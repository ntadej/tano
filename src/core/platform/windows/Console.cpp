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

#include "Common.h"
#include "Out.h"
#include "platform/windows/Console.h"

#if defined(Q_OS_WIN32)
#   include <windows.h>
#endif

#if defined(Q_OS_WIN32)
void Tano::Windows::pauseConsole()
{
    if(getenv("PWD")) return; /* Cygwin shell or Wine */

    Out() << "\nPress the RETURN key to continue...\n";

    getchar();
    fclose(stdout);
}

void Tano::Windows::showConsole()
{
    if(getenv("PWD")) return; /* Cygwin shell or Wine */

    AllocConsole();

    SetConsoleOutputCP(GetACP());
    SetConsoleTitle(Tano::name().toLocal8Bit().data());

    freopen("CONOUT$", "w", stderr);
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
}
#endif
