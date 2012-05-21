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

#if defined(Qt5)
    #include <QtWidgets/QMenu>
    #include <QtWidgets/QWidgetAction>
#elif defined(Qt4)
    #include <QtGui/QMenu>
    #include <QtGui/QWidgetAction>
#endif

#include "EpgScheduleFull.h"
#include "ui_EpgScheduleFull.h"

#include "container/core/Channel.h"
#include "container/xmltv/XmltvChannel.h"
#include "container/xmltv/XmltvProgramme.h"
#include "playlist/PlaylistModel.h"
#include "ui/playlist/PlaylistFilterWidget.h"

EpgScheduleFull::EpgScheduleFull(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::EpgScheduleFull)
{
    ui->setupUi(this);
    ui->schedule->setIdentifier(Tano::Schedule);

    _menu = new QMenu(this);
    _action = new QWidgetAction(this);
    ui->playlist->filter()->show();
    _action->setDefaultWidget(ui->playlist->filter());
    _menu->addAction(_action);
    ui->buttonFilter->setMenu(_menu);

    connect(ui->playlist, SIGNAL(itemSelected(Channel *)), this, SLOT(channel(Channel *)));
    connect(ui->schedule, SIGNAL(itemSelected(XmltvProgramme *)), this, SIGNAL(itemSelected(XmltvProgramme *)));
}

EpgScheduleFull::~EpgScheduleFull()
{
    delete ui;
}

void EpgScheduleFull::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void EpgScheduleFull::channel(Channel *channel)
{
    ui->schedule->setPage(0);
    emit requestEpg(channel->xmltvId(), Tano::Schedule);
}

void EpgScheduleFull::refreshPlaylistModel()
{
    ui->playlist->refreshModel();
}

EpgScheduleChannel *EpgScheduleFull::schedule()
{
    return ui->schedule;
}

void EpgScheduleFull::setPlaylistModel(PlaylistModel *model)
{
    ui->playlist->setModel(model);
}
