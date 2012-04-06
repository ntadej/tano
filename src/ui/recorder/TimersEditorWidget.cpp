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

#include <QtGui/QMessageBox>

#include "container/core/Timer.h"

#include "TimersEditorWidget.h"
#include "ui_TimersEditorWidget.h"

TimersEditorWidget::TimersEditorWidget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::TimersEditorWidget)
{
    ui->setupUi(this);

    createConnections();
}

TimersEditorWidget::~TimersEditorWidget()
{
    delete ui;
}

void TimersEditorWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void TimersEditorWidget::createConnections()
{
    //connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(cancel()));
    //connect(ui->buttonSave, SIGNAL(clicked()), this, SLOT(write()));
    //connect(ui->buttonDelete, SIGNAL(clicked()), this, SLOT(deleteItem()));
}

void TimersEditorWidget::write()
{

}

void TimersEditorWidget::cancel()
{

}

void TimersEditorWidget::deleteItem()
{
    //_timersModel->deleteTimer(_currentTimer);
}

void TimersEditorWidget::edit(Timer *item)
{
    if (item == 0)
        return;

    _currentTimer = item;

    ui->checkBoxDisabled->setChecked(_currentTimer->state() == Tano::Disabled);
    ui->editName->setText(_currentTimer->name());
    ui->editChannel->setText(_currentTimer->channel());
    ui->editUrl->setText(_currentTimer->url());
    ui->editType->setCurrentIndex(_currentTimer->type());
    ui->editDate->setDate(_currentTimer->date());
    ui->editStartTime->setTime(_currentTimer->startTime());
    ui->editEndTime->setTime(_currentTimer->endTime());
}

void TimersEditorWidget::validate()
{
    if (QDateTime(ui->editDate->date(), ui->editEndTime->time()) < QDateTime::currentDateTime()) {
        _currentTimer->setState(Tano::Expired);
    } else if (ui->checkBoxDisabled->isChecked()) {
        _currentTimer->setState(Tano::Disabled);
    } else {
        _currentTimer->setState(Tano::Enabled);
    }
}
