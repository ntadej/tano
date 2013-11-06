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

#include <QtCore/QSettings>

#include "Common.h"
#include "application/Output.h"
#include "platform/Windows.h"

#include <windows.h>

void Tano::Windows::pauseConsole()
{
    if(getenv("PWD")) return; /* Cygwin shell or Wine */

    Output() << "\nPress the RETURN key to continue...\n";

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

QString Tano::Windows::machineGuid()
{
    QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Cryptography", QSettings::NativeFormat);
    return settings.value("MachineGuid").toString();
}
