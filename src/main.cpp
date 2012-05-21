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

#include <QtGui/QBitmap>

#if defined(Qt5)
    #include <QtWidgets/QSplashScreen>
#elif defined(Qt4)
    #include <QtGui/QSplashScreen>
#endif

#include "core/Common.h"
#include "core/Log.h"
#include "core/Settings.h"
#include "ui/MainWindow.h"
#include "ui/wizard/FirstRunWizard.h"

#if defined(Qt4)
#ifdef Q_WS_X11
    #include <X11/Xlib.h>
#endif
#endif

int main(int argc, char *argv[])
{
#if defined(Qt4)
#ifdef Q_WS_X11
    XInitThreads();
#endif
#else
    QCoreApplication::setAttribute(Qt::AA_X11InitThreads);
#endif
    QCoreApplication::setApplicationName(Tano::application());
    QCoreApplication::setApplicationVersion(Tano::version());

    Tano::setupLog();

    QtSingleApplication instance(argc, argv);
    if(instance.sendMessage(""))
        return 0;

	Settings *settings = new Settings();
	settings->readSettings();
	bool splashShow = settings->splash();
	if(!settings->configured() || settings->configurationVersion() != Tano::version()) {
		FirstRunWizard *wizard = new FirstRunWizard();
		wizard->exec();
		delete wizard;
	}
	delete settings;

    QPixmap pixmap(":/images/splash.png");
    QSplashScreen *splash = new QSplashScreen(pixmap);
    splash->setMask(pixmap.mask());
    if  (splashShow)
        splash->show();

	MainWindow mainWindow;
	instance.setActivationWindow(&mainWindow);

	mainWindow.show();

    splash->hide();
    delete splash;

	return instance.exec();
}
