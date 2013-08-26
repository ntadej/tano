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

#include <QtCore/QtPlugin>

#include "widgets/application/TanoApplication.h"
#include "widgets/MainWindow.h"

Q_IMPORT_PLUGIN(TanoConfig)

int main(int argc, char *argv[])
{
    if (!TanoApplication::preInit())
        return -10;

    TanoApplication instance(argc, argv);
    // Is another instance of the program is already running
    if (!instance.shouldContinue())
        return 0;

    if (!instance.postInit())
        return -10;

    MainWindow main(instance.arguments());
    main.show();

    QObject::connect(&instance, SIGNAL(activate()), &main, SLOT(single()));

    return instance.exec();
}
