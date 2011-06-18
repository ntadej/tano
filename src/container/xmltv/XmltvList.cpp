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

#include <QtCore/QStringList>

#include "container/xmltv/XmltvChannel.h"
#include "container/xmltv/XmltvList.h"

XmltvList::XmltvList(const QDate &date)
    : _date(date) { }

XmltvList::~XmltvList() { }

void XmltvList::addChannel(XmltvChannel *c)
{
    _channels.append(c);
}

XmltvChannel *XmltvList::channel(const QString &id)
{
    for(int i = 0; i < _channels.size(); i++) {
        if(_channels[i]->id() == id || _channels[i]->id().split(".")[0] == id) {
            return _channels[i];
        }
    }
}
