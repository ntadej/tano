/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak <ntadej@users.sourceforge.net>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <QtCore/QDebug>
#include <QtDeclarative/qdeclarative.h>
#include <QtGui/QVBoxLayout>

#include <vlc-qt/Instance.h>
#include <vlc-qt/MediaPlayer.h>
#include <vlc-qt/VideoWidget.h>

#include "core/Common.h"

#include "MainQml.h"
//#include "Video.h"

MainQml::MainQml(QWidget *parent)
	: QWidget(parent)
{
	//qmlRegisterType<VideoDisplay>("VideoDisplay", 0, 2, "VideoDisplay");

	_view = new QDeclarativeView(this);
	_view->setSource(QUrl("qrc:/qml/Main.qml"));
	_view->setResizeMode(QDeclarativeView::SizeRootObjectToView);

	VlcInstance *i = new VlcInstance(Common::libvlcArgs(), this);
	VlcVideoWidget *v = new VlcVideoWidget(this);
	VlcMediaPlayer *p = new VlcMediaPlayer(v->widgetId(), this);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(v);
	layout->addWidget(_view);
	setLayout(layout);

	p->open("udp://@232.4.1.1:5002");
	p->play();
}

MainQml::~MainQml() { }
