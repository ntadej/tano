/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
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

#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtGui/QApplication>
#include <QtGui/QGraphicsObject>

#include "MobileCore.h"
#include "MobilePlaylistHandler.h"

#include "core/LocaleManager.h"
#include "playlist/PlaylistFilterModel.h"

MobileCore::MobileCore(QObject *parent)
    : QObject(parent)
{
    _locale = new LocaleManager();

    _playlist = new MobilePlaylistHandler(this);
    PlaylistFilterModel *model =  _playlist->model();

    _window = new QDeclarativeView();
    _window->rootContext()->setContextProperty("channelsModel", model);
    _window->rootContext()->setContextProperty("playlistManager", _playlist);
    _window->setSource(QUrl("qrc:/main.qml"));

    connect(_window->engine(), SIGNAL(quit()), qApp, SLOT(quit()));

#ifdef __arm__
    _window->showFullScreen();
#else
    _window->show();
#endif
}

MobileCore::~MobileCore()
{
    delete _locale;
    delete _playlist;
    delete _window;
}
