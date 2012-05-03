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

    ui->labelSearch->hide();
    ui->editSearch->hide();
    ui->buttonClear->hide();
    ui->comboType->hide();
    ui->labelType->hide();
    ui->comboCategory->hide();
    ui->labelCategory->hide();
    ui->comboLanguage->hide();
    ui->labelLanguage->hide();

    _filterModel = new PlaylistFilterModel(this);
    _filterModel->setDynamicSortFilter(true);

    ui->playlistView->setModel(_filterModel);

    connect(ui->playlistView, SIGNAL(activated(QModelIndex)), this, SLOT(channelSelected(QModelIndex)));
    connect(ui->comboCategory, SIGNAL(currentIndexChanged(QString)), this, SLOT(processFilters()));
    connect(ui->comboLanguage, SIGNAL(currentIndexChanged(QString)), this, SLOT(processFilters()));
    connect(ui->comboType, SIGNAL(currentIndexChanged(QString)), this, SLOT(processFilters()));
    connect(ui->editSearch, SIGNAL(textChanged(QString)), this, SLOT(processFilters()));
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
        ui->comboCategory->setItemText(0, tr("All categories"));
        ui->comboLanguage->setItemText(0, tr("All languages"));
        processFilters();
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
    ui->buttonSearch->hide();

    ui->labelSearch->show();
    ui->editSearch->show();
    ui->buttonClear->show();
}

void PlaylistDisplayWidget::processFilters()
{
    QRegExp regExp(ui->editSearch->text(), Qt::CaseInsensitive);
    _filterModel->setFilterRegExp(regExp);
    _filterModel->setCategory(ui->comboCategory->currentText());
    _filterModel->setLanguage(ui->comboLanguage->currentText());
    _filterModel->setType(Tano::ChannelType(ui->comboType->currentIndex()));
}

void PlaylistDisplayWidget::refreshModel()
{
    ui->comboCategory->clear();
    ui->comboCategory->insertItem(0, tr("All categories"));
    ui->comboCategory->insertItems(1, _model->categories());

    ui->comboLanguage->clear();
    ui->comboLanguage->insertItem(0, tr("All languages"));
    ui->comboLanguage->insertItems(1, _model->languages());
}

void PlaylistDisplayWidget::setModel(PlaylistModel *model)
{
    _model = model;
    _filterModel->setSourceModel(model);
}
