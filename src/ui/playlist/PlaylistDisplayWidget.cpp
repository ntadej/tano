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

#include "PlaylistDisplayWidget.h"
#include "ui_PlaylistDisplayWidget.h"

#include "container/Channel.h"
#include "playlist/PlaylistFilterModel.h"
#include "playlist/PlaylistModel.h"
/*#include "playlist/CSVGenerator.h"
#include "playlist/JsGenerator.h"
#include "playlist/M3UGenerator.h"
#include "playlist/PlaylistHandler.h"
#include "playlist/TvheadendGenerator.h"
#include "playlist/XmltvIdGenerator.h"*/

PlaylistDisplayWidget::PlaylistDisplayWidget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::PlaylistDisplayWidget),
      _current(0)
{
    ui->setupUi(this);

    _filterModel = new PlaylistFilterModel(this);
    _filterModel->setDynamicSortFilter(true);

    ui->playlistView->setModel(_filterModel);

    connect(ui->playlistView, SIGNAL(activated(QModelIndex)), this, SLOT(channelClicked(QModelIndex)));
    connect(ui->comboCategory, SIGNAL(currentIndexChanged(QString)), this, SLOT(processFilters()));
    connect(ui->comboLanguage, SIGNAL(currentIndexChanged(QString)), this, SLOT(processFilters()));
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
        break;
    default:
        break;
    }
}

void PlaylistDisplayWidget::channelClicked(const QModelIndex &index)
{
    _current = _model->row(index.row());
    emit itemClicked(_current);
}

void PlaylistDisplayWidget::editMode()
{
    ui->comboCategory->hide();
    ui->labelCategory->hide();
    ui->comboLanguage->hide();
    ui->labelLanguage->hide();
}

void PlaylistDisplayWidget::processFilters()
{
    QRegExp regExp(ui->editSearch->text(), Qt::CaseInsensitive);
    _filterModel->setFilterRegExp(regExp);
    _filterModel->setCategory(ui->comboCategory->currentText());
    _filterModel->setLanguage(ui->comboLanguage->currentText());
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

void PlaylistDisplayWidget::setCurrentChannel(Channel *channel)
{
    _current = channel;
    ui->playlistView->setCurrentIndex(_model->indexFromItem(channel));
}

void PlaylistDisplayWidget::setModel(PlaylistModel *model)
{
    _model = model;
    _filterModel->setSourceModel(model);
}
