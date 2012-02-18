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

#include <QtCore/QDate>

#include "container/xmltv/XmltvChannel.h"
#include "container/xmltv/XmltvProgramme.h"
#include "xmltv/XmltvCommon.h"
#include "xmltv/XmltvProgrammeFilterModel.h"
#include "xmltv/XmltvProgrammeModel.h"

#include "EpgScheduleChannel.h"
#include "ui_EpgScheduleChannel.h"

EpgScheduleChannel::EpgScheduleChannel(QWidget *parent)
	: QStackedWidget(parent),
	ui(new Ui::EpgScheduleChannel)
{
	ui->setupUi(this);

    _filterModel = new XmltvProgrammeFilterModel(this);
    _filterModel->setDynamicSortFilter(true);
    ui->view->setModel(_filterModel);

    connect(ui->view, SIGNAL(clicked(QModelIndex)), this, SLOT(programmeClicked(QModelIndex)));
    connect(ui->comboDate, SIGNAL(currentIndexChanged(QString)), this, SLOT(processFilters()));
}

EpgScheduleChannel::~EpgScheduleChannel()
{
	delete ui;
    delete _filterModel;
}

void EpgScheduleChannel::changeEvent(QEvent *e)
{
	QStackedWidget::changeEvent(e);
	switch (e->type()) {
		case QEvent::LanguageChange:
			ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

void EpgScheduleChannel::processFilters()
{
    _filterModel->setDate(QDate::fromString(ui->comboDate->currentText(), Tano::Xmltv::dateFormatDisplay()));
}

void EpgScheduleChannel::programmeClicked(const QModelIndex &index)
{
    emit itemSelected(_model->row(_filterModel->mapToSource(index).row()));
}

void EpgScheduleChannel::setEpg(XmltvProgrammeModel *epg,
                                const Tano::Id &id)
{
	if (id != _id)
		return;

    if (epg->rowCount() == 0)
        return;

    _model = epg;
    _filterModel->setSourceModel(_model);

    QList<QDate> date;
    for (int i = 0; i < _model->rowCount(); i++) {
        if (!date.contains(_model->row(i)->start().date()) && _model->row(i)->start().date() >= QDate::currentDate()) {
            date << _model->row(i)->start().date();
        }
    }

    ui->comboDate->clear();
    foreach (QDate d, date) {
        ui->comboDate->addItem(d.toString(Tano::Xmltv::dateFormatDisplay()));
    }

    setPage(1);
}

void EpgScheduleChannel::setPage(const int &id)
{
	setCurrentIndex(id);
}
