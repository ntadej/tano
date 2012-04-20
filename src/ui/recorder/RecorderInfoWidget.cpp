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

#include <QtCore/qmath.h>
#include <QtCore/QTime>
#include <QtGui/QMessageBox>

#include "container/core/Timer.h"

#include "RecorderInfoWidget.h"
#include "ui_RecorderInfoWidget.h"

RecorderInfoWidget::RecorderInfoWidget(QWidget *parent)
    : QStackedWidget(parent),
      ui(new Ui::RecorderInfoWidget),
      _actionRecord(0)
{
    ui->setupUi(this);
    ui->valueFile->removeBorder();
    ui->valueRFile->removeBorder();

    connect(ui->buttonRBack, SIGNAL(clicked()), this, SLOT(backToMain()));
    connect(ui->buttonTBack, SIGNAL(clicked()), this, SLOT(backToMain()));

    connect(ui->buttonRDelete, SIGNAL(clicked()), this, SLOT(recordingDelete()));
    connect(ui->buttonRPlay, SIGNAL(clicked()), this, SLOT(recordingPlay()));

    connect(ui->buttonTDelete, SIGNAL(clicked()), this, SLOT(timerDelete()));
    connect(ui->buttonTSave, SIGNAL(clicked()), this, SLOT(timerSave()));
}

RecorderInfoWidget::~RecorderInfoWidget()
{
    delete ui;
}

void RecorderInfoWidget::changeEvent(QEvent *e)
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

void RecorderInfoWidget::backToMain()
{
    _currentTimer = 0;

    setCurrentIndex(0);
}

void RecorderInfoWidget::recordingDelete()
{
    int r;
    r = QMessageBox::warning(this, tr("Recorder"),
                             tr("Are you sure you want to delete the recording?\nThis operation is ireversible."),
                             QMessageBox::Ok | QMessageBox::Cancel,
                             QMessageBox::Cancel);

    switch (r)
    {
    case QMessageBox::Ok:
        emit deleteRecording(_currentTimer);
        backToMain();
        break;
    case QMessageBox::Cancel:
    default:
        break;
    }
}

void RecorderInfoWidget::recordingInfo(Timer *timer)
{
    setCurrentIndex(1);

    _currentTimer = timer;

    qreal duration = timer->startTime().secsTo(timer->endTime());

    QString bold("<b>%1</b>");
    ui->valueRName->setText(bold.arg(timer->name()));
    ui->valueRChannel->setText(bold.arg(timer->channel()));
    ui->valueRTime->setText(bold.arg(QString("%1 %2 %3").arg(timer->date().toString("dd.MM.yyyy"), tr("at"), timer->startTime().toString("hh:mm"))));
    ui->valueRDuration->setText(bold.arg(QTime(0, qCeil(duration/60)).toString("hh:mm")));
    ui->valueRFile->setText(bold.arg(timer->file()));
}

void RecorderInfoWidget::recordingPlay()
{
    emit playRecording(_currentTimer);
    backToMain();
}

void RecorderInfoWidget::setAction(QAction *action)
{
    _actionRecord = action;
    connect(ui->buttonRecordStop, SIGNAL(clicked()), _actionRecord, SLOT(trigger()));
}

void RecorderInfoWidget::start(const QString &name,
                               const QString &channel,
                               const QString &output,
                               const QString &end)
{
    QString bold("<b>%1</b>");
    ui->valueCurrent->setText(bold.arg(name));
    ui->valueChannel->setText(bold.arg(channel));
    if (!end.isEmpty())
        ui->valueEndTime->setText(bold.arg(end));
    else
        ui->valueEndTime->setText(bold.arg(tr("No timer - stop manually.")));
    ui->valueFile->setText(bold.arg(output));

    ui->buttonRecordStop->setEnabled(true);
    ui->buttonRecordStop->setText(tr("Stop recording"));
}

void RecorderInfoWidget::stop()
{
    ui->valueCurrent->setText("");
    ui->valueChannel->setText("");
    ui->valueTime->setText("");
    ui->valueEndTime->setText(tr(""));
    ui->valueFile->setText("");

    ui->buttonRecordStop->setEnabled(false);
    ui->buttonRecordStop->setText(tr("Currently not recording."));
}

void RecorderInfoWidget::time(const int &time)
{
    QString bold("<b>%1</b>");
    ui->valueTime->setText(bold.arg(QTime().addMSecs(time).toString("hh:mm:ss")));
}

void RecorderInfoWidget::timerInfo(Timer *timer)
{
    setCurrentIndex(2);

    _currentTimer = timer;

    ui->editor->edit(timer);
}

void RecorderInfoWidget::timerDelete()
{
    emit deleteTimer(_currentTimer);

    backToMain();
}

void RecorderInfoWidget::timerSave()
{
    emit saveTimer(_currentTimer);

    backToMain();
}

void RecorderInfoWidget::timerSaveConfirm()
{
    ui->editor->save();
}
