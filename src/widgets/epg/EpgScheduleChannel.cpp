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

#if defined(Qt5)
    #include <QtWidgets/QAction>
    #include <QtWidgets/QMenu>
#elif defined(Qt4)
    #include <QtGui/QAction>
    #include <QtGui/QMenu>
#endif

#include "core/xmltv/XmltvCommon.h"
#include "core/xmltv/containers/XmltvChannel.h"
#include "core/xmltv/containers/XmltvProgramme.h"
#include "core/xmltv/models/XmltvProgrammeFilterModel.h"
#include "core/xmltv/models/XmltvProgrammeModel.h"

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
    ui->view->setContextMenuPolicy(Qt::CustomContextMenu);

	_rightMenu = new QMenu(ui->view);
    _info = new QAction(QIcon::fromTheme("x-office-calendar"), tr("Show information"), this);
    _record = new QAction(QIcon::fromTheme("media-record"), tr("Record"), this);
	_rightMenu->addAction(_info);
	_rightMenu->addAction(_record);

    ui->labelNoEpgIcon->setPixmap(QIcon::fromTheme("list-remove").pixmap(32));

    connect(ui->view, SIGNAL(activated(QModelIndex)), this, SLOT(programmeClicked(QModelIndex)));
    connect(ui->view, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showMenu(QPoint)));
    connect(ui->comboDate, SIGNAL(currentIndexChanged(QString)), this, SLOT(processFilters()));
    connect(_info, SIGNAL(triggered()), this, SLOT(info()));
    connect(_record, SIGNAL(triggered()), this, SLOT(record()));
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

void EpgScheduleChannel::info()
{
	programmeClicked(ui->view->indexAt(_currentPos));
}

void EpgScheduleChannel::processFilters()
{
    _filterModel->setDate(QDate::fromString(ui->comboDate->currentText(), Tano::Xmltv::dateFormatDisplay()));
}

void EpgScheduleChannel::programmeClicked(const QModelIndex &index)
{
    emit itemSelected(_model->row(_filterModel->mapToSource(index).row()));
}

void EpgScheduleChannel::record()
{
    emit requestRecord(_model->row(_filterModel->mapToSource(ui->view->indexAt(_currentPos)).row()));
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
    _filterModel->sort(0);

    QList<QDate> date;
    for (int i = 0; i < _model->rowCount(); i++) {
        if (!date.contains(_model->row(i)->start().date()) && _model->row(i)->start().date() >= QDate::currentDate()) {
            date << _model->row(i)->start().date();
        }
    }

    ui->comboDate->clear();
    foreach (const QDate &d, date) {
        ui->comboDate->addItem(d.toString(Tano::Xmltv::dateFormatDisplay()));
    }

    setPage(1);
}

void EpgScheduleChannel::setPage(const int &id)
{
	setCurrentIndex(id);
}

void EpgScheduleChannel::showMenu(const QPoint &pos)
{
	_currentPos = pos;

	_rightMenu->exec(QCursor::pos());
}
