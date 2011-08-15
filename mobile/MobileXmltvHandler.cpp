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

#include <QtCore/QDebug>
#include <QtGui/QStyledItemDelegate>

#include "MobileXmltvHandler.h"

#include "container/xmltv/XmltvProgramme.h"
#include "epg/XmltvCommon.h"
#include "epg/XmltvManager.h"
#include "epg/XmltvProgrammeFilterModel.h"
#include "epg/XmltvProgrammeModel.h"

MobileXmltvHandler::MobileXmltvHandler(QObject *parent)
    : QObject(parent)
{
    _xmltv = new XmltvManager(this);
    _filterModel = new XmltvProgrammeFilterModel(this);
    _filterModel->setDynamicSortFilter(true);

    connect(_xmltv, SIGNAL(epgSchedule(XmltvProgrammeModel*, Tano::Id)), this, SLOT(epgSchedule(XmltvProgrammeModel*, Tano::Id)));
}

MobileXmltvHandler::~MobileXmltvHandler()
{
    delete _xmltv;
    delete _filterModel;
}

QVariantList MobileXmltvHandler::dates()
{
    QVariantList list;
    for(int i = 0; i < _dates.size(); i++) {
        if(!_dates[i].isEmpty())
            list << _dates[i];
    }
    return list;
}

void MobileXmltvHandler::epgSchedule(XmltvProgrammeModel *model,
                                     const Tano::Id &id)
{
    Q_UNUSED(id)
    qDebug() << model->rowCount();
    if(model->rowCount() == 0)
        return;

    qDebug() << model->row(0)->channel();
    _filterModel->setSourceModel(model);
    _filterModel->setDate(model->row(0)->start().date());

    _dates.clear();
    _dateMap.clear();
    for(int i = 0; i < model->rowCount(); i++) {
        if(!_dates.contains(model->row(i)->start().date().toString(Tano::Xmltv::dateFormatMobile()))) {
            _dates << model->row(i)->start().date().toString(Tano::Xmltv::dateFormatMobile());
            _dateMap.insert(model->row(i)->start().date().toString(Tano::Xmltv::dateFormatMobile()), model->row(i)->start().date());
        }
    }
}

void MobileXmltvHandler::openXmltv(const QString &id)
{
    _xmltv->request(id, Tano::Schedule);
}

void MobileXmltvHandler::processDate(const QString &date)
{
    _filterModel->setDate(_dateMap[date]);
}
