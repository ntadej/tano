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

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeView>
#include <QtGui/QApplication>

#include "container/Channel.h"
#include "core/Common.h"
#include "core/ListModel.h"
#include "ui/playlist/PlaylistWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("Tano Mobile");

    //std::string property = "NoMStyle";
    //app.setProperty(property.c_str(), true);

    QDir::setCurrent(app.applicationDirPath());

    PlaylistWidget *playlist = new PlaylistWidget();
    playlist->hide();
    playlist->open(Tano::locateResource("playlists/sl/siol-mpeg4.m3u"));

    ListModel *model = new ListModel(new Channel, qApp);
    for(int i = 0; i < playlist->channels().size(); i++) {
        model->insertRow(playlist->channels()[i]->number(), playlist->channels()[i]);
    }

    QDeclarativeView window;
    window.rootContext()->setContextProperty("channelsModel",  model);
    window.setSource(QUrl("qrc:/main.qml"));

#ifdef __arm__
    window.showFullScreen();
#else
    window.show();
#endif
    return app.exec();
}
