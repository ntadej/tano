/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
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

#if QT_VERSION >= 0x050000
    #include <QtWidgets/QMessageBox>
#else
    #include <QtGui/QMessageBox>
#endif

#include "core/timers/containers/Timer.h"

#include "style/StyledBar.h"

#include "RecorderInfoWidget.h"
#include "ui_RecorderInfoWidget.h"

RecorderInfoWidget::RecorderInfoWidget(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::RecorderInfoWidget),
      _actionRecord(0)
{
    ui->setupUi(this);

    _labelInfo = new QLabel(this);
    _labelInfo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _labelInfo->setAlignment(Qt::AlignCenter);
    ui->toolbarTop->addWidget(_labelInfo);

    ui->toolbarBottom->setProperty("topBorder", true);
    ui->toolbarBottom->insertWidget(ui->actionRecording, new SimpleSeparator(this));
    ui->toolbarBottom->addWidget(new SimpleSeparator(this));

    connect(ui->actionBack, SIGNAL(triggered()), this, SLOT(backToMain()));
    connect(ui->actionDiscard, SIGNAL(triggered()), this, SLOT(backToMain()));
    connect(ui->actionDelete, SIGNAL(triggered()), this, SLOT(recordingDelete()));
    connect(ui->actionDelete, SIGNAL(triggered()), this, SLOT(timerDelete()));
    connect(ui->actionPlay, SIGNAL(triggered()), this, SLOT(recordingPlay()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(timerSave()));

    backToMain();
}

RecorderInfoWidget::~RecorderInfoWidget()
{
    delete ui;
}

void RecorderInfoWidget::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            _labelInfo->setText(tr("Recorder"));
            break;
        default:
            break;
    }
}

void RecorderInfoWidget::backToMain()
{
    _currentTimer = 0;

    _labelInfo->setText(tr("Recorder"));

    ui->editor->hide();
    ui->stack->setCurrentIndex(0);

    ui->actionBack->setVisible(false);
    ui->actionDelete->setVisible(false);
    ui->actionDiscard->setVisible(false);
    ui->actionPlay->setVisible(false);
    ui->actionSave->setVisible(false);

    ui->actionRecording->setVisible(true);
}

void RecorderInfoWidget::recordingDelete()
{
    if (ui->stack->currentWidget() != ui->pageRecording)
        return;

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
    ui->stack->setCurrentIndex(1);

    _labelInfo->setText(tr("Recording information"));

    ui->actionRecording->setVisible(false);
    ui->actionDiscard->setVisible(false);
    ui->actionSave->setVisible(false);
    ui->actionDelete->setVisible(true);
    ui->actionPlay->setVisible(true);
    ui->actionBack->setVisible(true);

    _currentTimer = timer;

    qreal duration = timer->startTime().secsTo(timer->endTime());

    QString bold("<b>%1</b>");
    ui->valueRChannel->setText(bold.arg(timer->channel()));
    ui->valueRTime->setText(bold.arg(QString("%1 %2 %3").arg(timer->date().toString("dd.MM.yyyy"), tr("at"), timer->startTime().toString("hh:mm"))));
    ui->valueRDuration->setText(bold.arg(QTime(0, qCeil(duration/60)).toString("hh:mm")));
}

void RecorderInfoWidget::recordingPlay()
{
    emit playRecording(_currentTimer);

    backToMain();
}

void RecorderInfoWidget::setAction(QAction *action)
{
    _actionRecord = action;
    connect(ui->actionRecording, SIGNAL(triggered()), _actionRecord, SLOT(trigger()));
}

void RecorderInfoWidget::setModel(TimersModel *model)
{
    ui->editor->setModel(model);
}

void RecorderInfoWidget::start(const QString &name,
                               const QString &channel,
                               const QString &end)
{
    QString bold("<b>%1</b>");
    ui->valueCurrent->setText(bold.arg(name));
    ui->valueChannel->setText(bold.arg(channel));
    if (!end.isEmpty())
        ui->valueEndTime->setText(bold.arg(end));
    else
        ui->valueEndTime->setText(bold.arg(tr("No timer - stop manually.")));

    ui->actionRecording->setEnabled(true);
    ui->actionRecording->setText(tr("Stop recording"));
}

void RecorderInfoWidget::stop()
{
    ui->valueCurrent->setText("");
    ui->valueChannel->setText("");
    ui->valueTime->setText("");
    ui->valueEndTime->setText(tr(""));

    ui->actionRecording->setEnabled(false);
    ui->actionRecording->setText(tr("Currently not recording."));
}

void RecorderInfoWidget::time(int time)
{
    QString bold("<b>%1</b>");
    ui->valueTime->setText(bold.arg(QTime().addMSecs(time).toString("hh:mm:ss")));
}

void RecorderInfoWidget::timerInfo(Timer *timer)
{
    ui->stack->setCurrentIndex(2);

    _labelInfo->setText(tr("Edit timer"));

    ui->actionRecording->setVisible(false);
    ui->actionBack->setVisible(false);
    ui->actionPlay->setVisible(false);
    ui->actionSave->setVisible(true);
    ui->actionDelete->setVisible(true);
    ui->actionDiscard->setVisible(true);

    _currentTimer = timer;

    ui->editor->show();
    ui->editor->edit(timer);
}

void RecorderInfoWidget::timerDelete()
{
    if (ui->stack->currentWidget() != ui->pageTimer)
        return;

    emit deleteTimer(_currentTimer);

    backToMain();
}

void RecorderInfoWidget::timerSave()
{
    if (!ui->editor->save())
        return;

    emit saveTimer(_currentTimer);

    backToMain();
}
