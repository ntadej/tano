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

#ifndef TANO_M3UGENERATOR_H_
#define TANO_M3UGENERATOR_H_

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "File.h"

class Channel;
class PlaylistModel;
class NetworkUdpxy;

class M3UGenerator
{
public:
    M3UGenerator(const QString &file,
                 const File::Type &type = File::M3U);
    ~M3UGenerator();

    bool write(PlaylistModel *model);

private:
    void generateItem(Channel *channel,
                      bool clean = false,
                      bool udpxy = false);

    File::Type _type;

    QFile *_file;
    QTextStream _out;

    NetworkUdpxy *_udpxy;

    QString _hd;
    QString _radio;
};

#endif // TANO_M3UGENERATOR_H_
