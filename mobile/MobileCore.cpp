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

#include "MobileConstants.h"
#include "MobileCore.h"
#include "MobilePlaylistHandler.h"
#include "MobileXmltvHandler.h"

#include "core/LocaleManager.h"
#include "epg/XmltvProgrammeFilterModel.h"
#include "playlist/PlaylistFilterModel.h"

MobileCore::MobileCore(QObject *parent)
    : QObject(parent),
      _playlist(0),
      _xmltv(0)
{
    _locale = new LocaleManager();
    _window = new QDeclarativeView();
    _window->rootContext()->setContextProperty("TanoType", MobileConstants::typeConstants());
    _window->rootContext()->setContextProperty("TanoUi", MobileConstants::uiConstants());
    _window->rootContext()->setContextProperty("core", this);
    _window->setSource(QUrl("qrc:/main.qml"));

#ifdef __arm__
    _window->showFullScreen();
#else
    _window->show();
#endif

    createPlaylist();
    //createXmltv();

    connect(_window->engine(), SIGNAL(quit()), qApp, SLOT(quit()));
}

MobileCore::~MobileCore()
{
    delete _locale;
    delete _playlist;
    delete _window;
    delete _xmltv;
}

void MobileCore::createPlaylist()
{
    if(_playlist)
        return;

    _playlist = new MobilePlaylistHandler(this);
    PlaylistFilterModel *pm =  _playlist->model();

    _window->rootContext()->setContextProperty("channelsModel", pm);
    _window->rootContext()->setContextProperty("playlistManager", _playlist);
}

void MobileCore::createXmltv()
{
    if(_xmltv)
        return;

    _xmltv = new MobileXmltvHandler(this);
    XmltvProgrammeFilterModel *xm = _xmltv->model();

    _window->rootContext()->setContextProperty("xmltvModel", xm);
    _window->rootContext()->setContextProperty("xmltvManager", _xmltv);
}
