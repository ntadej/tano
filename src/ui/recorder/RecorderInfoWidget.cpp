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

#include <QtCore/QTime>

#include "RecorderInfoWidget.h"
#include "ui_RecorderInfoWidget.h"

RecorderInfoWidget::RecorderInfoWidget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::RecorderInfoWidget),
      _actionRecord(0)
{
    ui->setupUi(this);
    ui->valueFile->removeBorder();
}

RecorderInfoWidget::~RecorderInfoWidget()
{
    delete ui;
}

void RecorderInfoWidget::changeEvent(QEvent *e)
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
