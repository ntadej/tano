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
#include "playlist/PlaylistModel.h"
/*#include "playlist/CSVGenerator.h"
#include "playlist/JsGenerator.h"
#include "playlist/M3UGenerator.h"
#include "playlist/PlaylistHandler.h"
#include "playlist/TvheadendGenerator.h"
#include "playlist/XmltvIdGenerator.h"*/

PlaylistDisplayWidget::PlaylistDisplayWidget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::PlaylistDisplayWidget)
{
    ui->setupUi(this);
    //ui->treeWidget->header()->setResizeMode(QHeaderView::ResizeToContents);

    connect(ui->playlistView, SIGNAL(clicked(QModelIndex)), this, SLOT(channelClicked(QModelIndex)));
    //connect(ui->categoryBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(processPlaylist()));
    //connect(ui->languageBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(processPlaylist()));
    //connect(ui->searchEdit, SIGNAL(textChanged(QString)), this, SLOT(processPlaylist()));
}

PlaylistDisplayWidget::~PlaylistDisplayWidget()
{
    delete ui;
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
    emit itemClicked(_model->row(index.row()));
}

void PlaylistDisplayWidget::setModel(PlaylistModel *model)
{
    _model = model;
    ui->playlistView->setModel(model);
}
