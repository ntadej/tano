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

#include "PlaylistDisplayWidget.h"
#include "ui_PlaylistDisplayWidget.h"

#include "container/core/Channel.h"
#include "playlist/PlaylistFilterModel.h"
#include "playlist/PlaylistModel.h"

PlaylistDisplayWidget::PlaylistDisplayWidget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::PlaylistDisplayWidget),
      _current(0)
{
    ui->setupUi(this);
    ui->filters->hide();

    _filterModel = new PlaylistFilterModel(this);
    _filterModel->setDynamicSortFilter(true);

    ui->playlistView->setModel(_filterModel);

    connect(ui->playlistView, SIGNAL(activated(QModelIndex)), this, SLOT(channelSelected(QModelIndex)));
    connect(ui->filters, SIGNAL(filters(QString, QString, QString, int)), this, SLOT(processFilters(QString, QString, QString, int)));
}

PlaylistDisplayWidget::~PlaylistDisplayWidget()
{
    delete ui;
    delete _filterModel;
}

void PlaylistDisplayWidget::changeEvent(QEvent *e)
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

void PlaylistDisplayWidget::channelSelected(Channel *channel)
{
    _current = channel;
    emit itemSelected(_current);
}

void PlaylistDisplayWidget::channelSelected(const QModelIndex &index)
{
    _current = _model->row(_filterModel->mapToSource(index).row());
    emit itemSelected(_current);
}

void PlaylistDisplayWidget::channelSelected(const int &channel)
{
    _current = _model->number(channel);
    emit itemSelected(_current);
}

void PlaylistDisplayWidget::channelSelected(const QString &xmltvId)
{
    _current = _model->xmltvId(xmltvId);
    emit itemSelected(_current);
}

void PlaylistDisplayWidget::editMode()
{
    ui->filters->editMode();
    ui->filters->show();
}

PlaylistFilterWidget *PlaylistDisplayWidget::filter()
{
    return ui->filters;
}

void PlaylistDisplayWidget::processFilters(const QString &search,
                                           const QString &category,
                                           const QString &language,
                                           const int &type)
{
    QRegExp regExp(search, Qt::CaseInsensitive);
    _filterModel->setFilterRegExp(regExp);
    _filterModel->setCategory(category);
    _filterModel->setLanguage(language);
    _filterModel->setType(Tano::ChannelType(type));
}

void PlaylistDisplayWidget::refreshModel()
{
    ui->filters->refreshModel(_model->categories(), _model->languages());
}

void PlaylistDisplayWidget::setModel(PlaylistModel *model)
{
    _model = model;
    _filterModel->setSourceModel(model);
}
