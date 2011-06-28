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

#ifndef TANO_M3UGENERATOR_H_
#define TANO_M3UGENERATOR_H_

#include <QtCore/QTextStream>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>

#include "ui/core/FileDialogs.h"

class Channel;
class Udpxy;

class M3UGenerator
{
public:
    M3UGenerator(QTreeWidget *treeWidget,
                 const QString &name,
                 QMap<QTreeWidgetItem *, Channel *> map,
                 const FileDialogs::Type &type = FileDialogs::M3U);
    ~M3UGenerator();

    bool write(QIODevice *device);

private:
    void generateItemNormal(Channel *channel);
    void generateItemClean(Channel *channel);
    void generateItemUdpxy(Channel *channel);

    FileDialogs::Type _type;

    QTreeWidget *_treeWidget;
    QString _name;
    QTextStream _out;

    QMap<QTreeWidgetItem *, Channel *> _map;

    Udpxy *_udpxy;
};

#endif // TANO_M3UGENERATOR_H_
