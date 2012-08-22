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

#ifndef TANO_XMLTVCHANNELSMODEL_H_
#define TANO_XMLTVCHANNELSMODEL_H_

#include "container/ListModel.h"

class XmltvChannel;

class XmltvChannelsModel : public ListModel
{
Q_OBJECT
public:
    XmltvChannelsModel(QObject *parent = 0);
    ~XmltvChannelsModel();

    XmltvChannel *find(const QString &id) const;
    XmltvChannel *row(const int &row);
    XmltvChannel *takeRow(const int &row);
};

#endif // TANO_XMLTVCHANNELSMODEL_H_
