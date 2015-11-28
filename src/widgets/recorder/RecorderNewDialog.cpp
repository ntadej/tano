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

#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidgetAction>

#include "playlist/PlaylistModel.h"
#include "playlist/containers/Channel.h"
#include "timers/containers/Timer.h"
#include "timers/models/TimersModel.h"
#include "xmltv/containers/XmltvProgramme.h"

#include "RecorderNewDialog.h"
#include "ui_RecorderNewDialog.h"

RecorderNewDialog::RecorderNewDialog(bool quick,
                                     TimersModel *timers,
                                     PlaylistModel *playlist,
                                     QWidget *parent)
    : QDialog(parent),
      ui(new Ui::RecorderNewDialog),
      _currentChannel(0),
      _currentTimer(0)
{
    setWindowModality(Qt::WindowModal);

    ui->setupUi(this);

    _model = timers;
    ui->playlistWidget->setPlaylistModel(playlist);

    if (quick)
        ui->buttonTimer->hide();
    else
        ui->buttonRecord->hide();

    ui->search->setButtonPixmap(FancyLineEdit::Left, QIcon::fromTheme("edit-find").pixmap(16));
    ui->search->setButtonPixmap(FancyLineEdit::Right, QIcon::fromTheme("edit-clear").pixmap(16));
    ui->search->setButtonVisible(FancyLineEdit::Left, true);
    ui->search->setButtonVisible(FancyLineEdit::Right, true);
    ui->search->setAutoHideButton(FancyLineEdit::Right, true);

    connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->buttonTimer, SIGNAL(clicked()), this, SLOT(processNewTimer()));
    connect(ui->buttonRecord, SIGNAL(clicked()), this, SLOT(processQuickRecord()));
    connect(ui->editName, SIGNAL(textChanged(QString)), this, SLOT(validate()));
    connect(ui->search, SIGNAL(textChanged(QString)), this, SLOT(processFilters()));
    connect(ui->search, SIGNAL(rightButtonClicked()), ui->search, SLOT(clear()));

    connect(ui->playlistWidget, SIGNAL(itemSelected(Channel *)), this, SLOT(playlist(Channel *)));
}

RecorderNewDialog::~RecorderNewDialog()
{
    delete ui;
}

void RecorderNewDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

Timer *RecorderNewDialog::newTimerFromSchedule(XmltvProgramme *programme)
{
    ui->playlistWidget->channelSelected(programme->channel());

    if (!_currentChannel) {
        QMessageBox::critical(this, tr("Recorder"),
                    tr("You don't have this channel in your playlist."));
        return 0;
    }

    Timer *timer = _model->createTimer(programme->title(), _currentChannel->id());
    timer->setState(Timer::Disabled);
    timer->setDate(QDateTime::fromTime_t(programme->start()).date());
    timer->setStartTime(QDateTime::fromTime_t(programme->start()).time());
    timer->setEndTime(QDateTime::fromTime_t(programme->stop()).time());

    _currentTimer = timer;

    return timer;
}

void RecorderNewDialog::playlist(Channel *channel)
{
    _currentChannel = channel;

    ui->valueSelected->setText("<b>" + channel->name() + "</b>");

    validate();
}

void RecorderNewDialog::processFilters()
{
    ui->playlistWidget->setFilters(ui->search->text());
}

void RecorderNewDialog::processNewTimer()
{
    Timer *timer = _model->createTimer(ui->editName->text(), _currentChannel->id());
    timer->setState(Timer::Disabled);

    _currentTimer = timer;

    accept();
}

void RecorderNewDialog::processQuickRecord()
{
    Timer *timer = _model->createTimer(ui->editName->text(), _currentChannel->id(), Timer::Instant);

    _currentTimer = timer;

    accept();
}

void RecorderNewDialog::validate()
{
    if (!ui->editName->text().isEmpty() && !ui->valueSelected->text().isEmpty()) {
        ui->buttonRecord->setEnabled(true);
        ui->buttonTimer->setEnabled(true);
    } else {
        ui->buttonRecord->setEnabled(false);
        ui->buttonTimer->setEnabled(false);
    }
}
