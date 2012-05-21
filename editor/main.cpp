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

#include <QtCore/QCoreApplication>
#include <QtGui/QBitmap>

#if defined(Qt5)
    #include <QtWidgets/QApplication>
    #include <QtWidgets/QSplashScreen>
#elif defined(Qt4)
    #include <QtGui/QApplication>
    #include <QtGui/QSplashScreen>
#endif

#include "Config.h"
#include "core/Common.h"
#include "core/Log.h"
#include "core/Settings.h"
#include "ui/playlist/PlaylistEditor.h"

#if defined(Qt4)
#ifdef Q_WS_X11
    #include <X11/Xlib.h>
#endif
#endif

#if WITH_EDITOR_VLCQT
    #include <vlc-qt/Common.h>
    #include <vlc-qt/Instance.h>
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

    QApplication app(argc, argv);

	Settings *settings = new Settings();
	bool splashShow = settings->splash();
	delete settings;

    QPixmap pixmap(":/images/splash-editor.png");
    QSplashScreen *splash = new QSplashScreen(pixmap);
    splash->setMask(pixmap.mask());
    if  (splashShow)
        splash->show();

    PlaylistEditor main;
#if WITH_EDITOR_VLCQT
    VlcInstance *instance = new VlcInstance(VlcCommon::args());
    main.setMediaInstance(instance);
#endif
    main.show();

    splash->hide();
    delete splash;

    return app.exec();
}
