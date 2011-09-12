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

#include <QtCore/QFile>
#include <QtGui/QCloseEvent>
#include <QtGui/QMessageBox>

#include "TimersEdit.h"
#include "ui_TimersEdit.h"

#include "container/core/Channel.h"
#include "container/core/Timer.h"
#include "core/Common.h"
#include "core/Settings.h"
#include "playlist/PlaylistModel.h"
#include "recorder/TimersModel.h"

TimersEdit::TimersEdit(PlaylistModel *playlist,
                       QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::TimersEdit),
      _channel(0),
      _closeEnabled(false)
{
    ui->setupUi(this);

    _path = Tano::settingsPath();

    ui->dockWidgetContents->setDisabled(true);
    _playlistModel = playlist;
    ui->playlistWidget->setModel(playlist);
    ui->playlistWidget->refreshModel();

    createSettings();
    createConnections();

    _timersModel = new TimersModel(this);
    ui->timersWidget->setModel(_timersModel);

    read();
}

TimersEdit::~TimersEdit()
{
    delete ui;
    delete _timersModel;
}

void TimersEdit::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void TimersEdit::closeEvent(QCloseEvent *event)
{
    event->ignore();
    exit();
}

void TimersEdit::createSettings()
{
    Settings *settings = new Settings(this);
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonStyle(settings->toolbarLook()));
    delete settings;
}

void TimersEdit::createConnections()
{
    connect(ui->timersWidget, SIGNAL(itemSelected(Timer *)), this, SLOT(edit(Timer *)));
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newItem()));
    connect(ui->actionDelete, SIGNAL(triggered()), this, SLOT(deleteItem()));
    connect(ui->buttonCreate, SIGNAL(clicked()), this, SLOT(addItem()));
    connect(ui->playlistWidget, SIGNAL(itemSelected(Channel *)), this, SLOT(playlist(Channel *)));

    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(write()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(exit()));

    connect(ui->editName, SIGNAL(textChanged(QString)), this, SLOT(editName(QString)));
    connect(ui->editType, SIGNAL(currentIndexChanged(int)), this, SLOT(editType(int)));
    connect(ui->editDate, SIGNAL(dateChanged(QDate)), this, SLOT(editDate(QDate)));
    connect(ui->editStartTime, SIGNAL(timeChanged(QTime)), this, SLOT(editStartTime(QTime)));
    connect(ui->editEndTime, SIGNAL(timeChanged(QTime)), this, SLOT(editEndTime(QTime)));
    connect(ui->checkBoxDisabled, SIGNAL(clicked()), this, SLOT(validate()));
}

void TimersEdit::exit()
{
    if(_closeEnabled) {
        hide();
        return;
    }

    int c;
    c = QMessageBox::warning(this, tr("Timers Editor"),
                                   tr("Do you want close the editor?\nYou changes will not be applied."),
                                   QMessageBox::Save | QMessageBox::Close | QMessageBox::Cancel,
                                   QMessageBox::Save);

    switch (c) {
        case QMessageBox::Save:
            ui->actionSave->trigger();
            break;
        case QMessageBox::Close:
            _closeEnabled = true;
            ui->actionClose->trigger();
            break;
        case QMessageBox::Cancel:
            break;
        default:
            break;
    }
}

void TimersEdit::read()
{
    QString fileName = _path + "timers.tano.xml";

    QFile file(fileName);
    if(!file.exists())
        return;

    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Tano"),
                            tr("Cannot read file %1:\n%2.")
                            .arg(fileName)
                            .arg(file.errorString()));
        return;
    }
    file.close();

    _timersModel->readTimers(fileName);
}

void TimersEdit::write()
{
    QString fileName = _path + "timers.tano.xml";

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Tano"),
                            tr("Cannot write file %1:\n%2.")
                            .arg(fileName)
                            .arg(file.errorString()));
        return;
    }

    _timersModel->writeTimers(fileName);

    _closeEnabled = true;
    emit updateTimers();
    exit();
}

void TimersEdit::newItem()
{
    ui->toolBar->setDisabled(true);
    ui->dockWidgetContents->setDisabled(true);
    ui->mainWidget->setCurrentIndex(1);
}

void TimersEdit::deleteItem()
{
    ui->dockWidgetContents->setDisabled(true);
    _timersModel->deleteTimer(ui->timersWidget->currentTimer());
}

void TimersEdit::addItem()
{
    if(!_channel || ui->editNameNew->text().isEmpty()) {
        QMessageBox::warning(this, tr("Tano"),
                            tr("Please enter a name and select a channel from the list."));
        return;
    } else {
        for(int i = 0; i < _timersModel->rowCount(); i++) {
            if(_timersModel->row(i)->name() == ui->editNameNew->text()) {
                QMessageBox::warning(this, tr("Tano"),
                                    tr("Timer with this name already exists. Please select another name."));
                return;
            }
        }
    }

    edit(_timersModel->createTimer(ui->editNameNew->text(), _channel->name(), _playlistModel->fileName(), _channel->number(), _channel->url()));

    ui->dockWidgetContents->setDisabled(false);
    ui->toolBar->setDisabled(false);
    ui->mainWidget->setCurrentIndex(0);
}

void TimersEdit::playlist(Channel *channel)
{
    _channel = channel;
}

void TimersEdit::edit(Timer *item)
{
    if(item == 0)
        return;

    ui->dockWidgetContents->setDisabled(false);

    ui->timersWidget->setCurrentTimer(item);

    ui->checkBoxDisabled->setChecked(ui->timersWidget->currentTimer()->isDisabled());
    ui->editName->setText(ui->timersWidget->currentTimer()->name());
    ui->editChannel->setText(ui->timersWidget->currentTimer()->channel());
    ui->editNum->display(ui->timersWidget->currentTimer()->num());
    ui->editPlaylist->setText(ui->timersWidget->currentTimer()->playlist());
    ui->editUrl->setText(ui->timersWidget->currentTimer()->url());
    ui->editType->setCurrentIndex(ui->timersWidget->currentTimer()->type());
    ui->editDate->setDate(ui->timersWidget->currentTimer()->startTime().date());
    ui->editStartTime->setTime(ui->timersWidget->currentTimer()->startTime().time());
    ui->editEndTime->setTime(ui->timersWidget->currentTimer()->endTime().time());
}

void TimersEdit::editName(const QString &name)
{
    for(int i = 0; i < _timersModel->rowCount(); i++)
        if(_timersModel->row(i)->name() == name && _timersModel->row(i) != ui->timersWidget->currentTimer()) {
            QMessageBox::warning(this, tr("Tano"),
                                tr("Timer with this name already exists. Please select another name."));
            ui->editName->setText(ui->timersWidget->currentTimer()->name());
            return;
        }

    ui->timersWidget->currentTimer()->setName(name);
}

void TimersEdit::editType(const int &type)
{
    ui->timersWidget->currentTimer()->setType(Tano::TimerType(type));
}

void TimersEdit::editDate(const QDate &date)
{
    QDateTime start = ui->timersWidget->currentTimer()->startTime();
    QDateTime end = ui->timersWidget->currentTimer()->endTime();
    start.setDate(date);
    end.setDate(date);
    ui->timersWidget->currentTimer()->setStartTime(start);
    ui->timersWidget->currentTimer()->setEndTime(end);
}

void TimersEdit::editStartTime(const QTime &time)
{
    QDateTime start = ui->timersWidget->currentTimer()->startTime();
    start.setTime(time);
    ui->timersWidget->currentTimer()->setStartTime(start);
    validate();
}

void TimersEdit::editEndTime(const QTime &time)
{
    QDateTime end = ui->timersWidget->currentTimer()->endTime();
    end.setTime(time);
    ui->timersWidget->currentTimer()->setEndTime(end);
    validate();
}

void TimersEdit::validate()
{
    if((QDateTime(ui->editDate->date(), ui->editEndTime->time()) < QDateTime::currentDateTime()) ||
       ui->checkBoxDisabled->isChecked())
    {
        ui->timersWidget->currentTimer()->setDisabled(true);
        ui->checkBoxDisabled->setChecked(true);
    } else {
        ui->timersWidget->currentTimer()->setDisabled(false);
    }
}
