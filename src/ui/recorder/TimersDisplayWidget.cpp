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

#include "TimersDisplayWidget.h"
#include "ui_TimersDisplayWidget.h"

#include "container/core/Timer.h"
#include "recorder/TimersFilterModel.h"
#include "recorder/TimersModel.h"

TimersDisplayWidget::TimersDisplayWidget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::TimersDisplayWidget),
      _current(0)
{
    ui->setupUi(this);

    _filterModel = new TimersFilterModel(this);
    _filterModel->setDynamicSortFilter(true);

    ui->timersView->setModel(_filterModel);

    connect(ui->timersView, SIGNAL(activated(QModelIndex)), this, SLOT(timerClicked(QModelIndex)));
    connect(ui->editSearch, SIGNAL(textChanged(QString)), this, SLOT(processFilters()));
}

TimersDisplayWidget::~TimersDisplayWidget()
{
    delete ui;
    delete _filterModel;
}

void TimersDisplayWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void TimersDisplayWidget::processFilters()
{
    QRegExp regExp(ui->editSearch->text(), Qt::CaseInsensitive);
    _filterModel->setFilterRegExp(regExp);
}

void TimersDisplayWidget::setCurrentTimer(Timer *timer)
{
    _current = timer;
    ui->timersView->setCurrentIndex(_model->indexFromItem(timer));
}

void TimersDisplayWidget::setModel(TimersModel *model)
{
    _model = model;
    _filterModel->setSourceModel(model);
}

void TimersDisplayWidget::timerClicked(const QModelIndex &index)
{
    _current = _model->row(_filterModel->mapToSource(index).row());
    emit itemClicked(_current);
}
