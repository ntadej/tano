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

#include <QApplication>
#include <QTextCodec>

#include "core/Common.h"
#include "core/Log.h"
#include "core/Out.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName(Tano::name());
    QCoreApplication::setApplicationVersion(Tano::version());

    QCoreApplication::setAttribute(Qt::AA_X11InitThreads);

#if QT_VERSION < 0x050000
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif

    Tano::Log::setup();

    QApplication instance(argc, argv);

    Out::welcome();

    // TODO: Playlist editor
    /*if (_playlistEditor) {
        if (_playlistEditor->isVisible()) {
            _playlistEditor->activateWindow();
        } else {
            disconnect(_xmltv, SIGNAL(channelsChanged(QHash<QString, QString>)), _playlistEditor, SLOT(setXmltvMap(QHash<QString, QString>)));
            delete _playlistEditor;
            _playlistEditor = new PlaylistEditor(this);
            _playlistEditor->setMediaInstance(_mediaInstance);
            _playlistEditor->setXmltvMap(_xmltv->channels());
            _playlistEditor->open(_playlistName);
            _playlistEditor->show();
        }
    } else {
        _playlistEditor = new PlaylistEditor(this);
        _playlistEditor->setMediaInstance(_mediaInstance);
        connect(_xmltv, SIGNAL(channelsChanged(QHash<QString, QString>)), _playlistEditor, SLOT(setXmltvMap(QHash<QString, QString>)));
        _playlistEditor->setXmltvMap(_xmltv->channels());
        _playlistEditor->open(_playlistName);
        _playlistEditor->show();
    }*/

    return 0;
}
