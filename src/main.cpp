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

#include <QtSingleApplication>

#include <QtCore/QTextCodec>

#include "core/Arguments.h"
#include "core/Common.h"
#include "core/Log.h"
#include "ui/MainWindow.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName(Tano::name());
    QCoreApplication::setApplicationVersion(Tano::version());

    QCoreApplication::setAttribute(Qt::AA_X11InitThreads);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    Tano::Log::setup();

    QtSingleApplication instance(argc, argv);
    if(instance.sendMessage(""))
        return 0;

    Arguments arguments(argc, argv);

    MainWindow mainWindow;
    instance.setActivationWindow(&mainWindow);

    mainWindow.show();

	return instance.exec();
}
