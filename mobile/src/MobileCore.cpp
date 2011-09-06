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

#include "qmlapplicationviewer/qmlapplicationviewer.h"

#include "MobileConstants.h"
#include "MobileCore.h"
#include "MobileXmltvHandler.h"

#include "core/LocaleManager.h"
#include "epg/XmltvProgrammeFilterModel.h"

MobileCore::MobileCore(QObject *parent)
    : QObject(parent),
      _xmltv(0)
{
    _locale = new LocaleManager();
    _viewer = new QmlApplicationViewer();
    _viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    _viewer->rootContext()->setContextProperty("TanoUi", MobileConstants::uiConstants());
    _viewer->rootContext()->setContextProperty("TanoCore", this);
    _viewer->setMainQmlFile(QLatin1String("qml/main.qml"));

    _viewer->showExpanded();

    //createXmltv();

    connect(_viewer->engine(), SIGNAL(quit()), qApp, SLOT(quit()));
}

MobileCore::~MobileCore()
{
    delete _locale;
    delete _viewer;
    delete _xmltv;
}

void MobileCore::createXmltv()
{
    if(_xmltv)
        return;

    _xmltv = new MobileXmltvHandler(this);
    XmltvProgrammeFilterModel *xm = _xmltv->model();

    _viewer->rootContext()->setContextProperty("XmltvProgramme", xm);
    _viewer->rootContext()->setContextProperty("XmltvManager", _xmltv);
}
