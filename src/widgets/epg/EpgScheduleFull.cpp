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

#include "core/playlist/PlaylistModel.h"
#include "core/playlist/containers/Channel.h"
#include "core/xmltv/containers/XmltvChannel.h"
#include "core/xmltv/containers/XmltvProgramme.h"

#include "common/OsdFloat.h"
#include "playlist/PlaylistFilterWidget.h"

EpgScheduleFull::EpgScheduleFull(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::EpgScheduleFull),
      _fullscreen(false),
      _osd(0)
{
    ui->setupUi(this);
    ui->schedule->setIdentifier(Tano::Schedule);

    _menu = new QMenu(this);
    _action = new QWidgetAction(this);
    ui->playlist->filter()->show();
    _action->setDefaultWidget(ui->playlist->filter());
    _menu->addAction(_action);
    ui->buttonFilter->setMenu(_menu);
    ui->buttonClose->hide();
    ui->labelTitle->hide();

    connect(ui->playlist, SIGNAL(itemSelected(Channel *)), this, SLOT(channel(Channel *)));
    connect(ui->schedule, SIGNAL(itemSelected(QString)), this, SIGNAL(itemSelected(QString)));
    connect(ui->schedule, SIGNAL(requestRecord(QString)), this, SIGNAL(requestRecord(QString)));
    connect(ui->buttonClose, SIGNAL(clicked()), this, SLOT(closeOsd()));;
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

void EpgScheduleFull::closeOsd()
{
    if (_fullscreen)
        _osd->floatHide();
}

void EpgScheduleFull::openSchedule(Channel *channel)
{
    if (channel)
        ui->playlist->channelSelected(channel);

    if (_fullscreen) {
        _osd->floatShow();
    } else {
        if (isVisible())
            activateWindow();
        else
            show();
    }
}

void EpgScheduleFull::refreshPlaylistModel()
{
    ui->playlist->refreshModel();
}

EpgScheduleChannel *EpgScheduleFull::schedule()
{
    return ui->schedule;
}

void EpgScheduleFull::setFullscreen(const bool &enabled,
                                    OsdFloat *widget)
{
    ui->buttonClose->setVisible(enabled);
    ui->labelTitle->setVisible(enabled);

    _fullscreen = enabled;

    if (enabled && widget) {
        _osd = widget;
        _osd->setWidget(ui->main);
    } else {
        _osd = 0;
        ui->layoutMain->addWidget(ui->main);
    }
}

void EpgScheduleFull::setPlaylistModel(PlaylistModel *model)
{
    ui->playlist->setModel(model);
}
