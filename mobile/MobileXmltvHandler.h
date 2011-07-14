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

#ifndef TANO_MOBILEXMLTVHANDLER_H_
#define TANO_MOBILEXMLTVHANDLER_H_

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QVariantList>

#include "core/Enums.h"

class Channel;
class XmltvManager;
class XmltvProgrammeFilterModel;
class XmltvProgrammeModel;

class MobileXmltvHandler : public QObject
{
Q_OBJECT
public:
    MobileXmltvHandler(QObject *parent = 0);
    ~MobileXmltvHandler();

    XmltvProgrammeFilterModel *model() { return _filterModel; }
    Q_INVOKABLE QVariantList dates();

    Q_INVOKABLE void openXmltv(const QString &id);
    Q_INVOKABLE void processDate(const QString &date);

private slots:
    void epgSchedule(XmltvProgrammeModel *model,
                     const Tano::Id &id);

private:
    XmltvManager *_xmltv;
    XmltvProgrammeFilterModel *_filterModel;

    QStringList _dates;
};

#endif // TANO_MOBILEXMLTVHANDLER_H_
