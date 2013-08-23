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

#if defined(Qt5)
    #include <QtWidgets/QMessageBox>
#elif defined(Qt4)
    #include <QtGui/QMessageBox>
#endif

#include "core/timers/containers/Timer.h"
#include "core/timers/models/TimersFilterModel.h"
#include "core/timers/models/TimersModel.h"

#include "RecorderTimersEditor.h"
#include "ui_RecorderTimersEditor.h"

RecorderTimersEditor::RecorderTimersEditor(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::RecorderTimersEditor)
{
    ui->setupUi(this);

    _validateModel = new TimersFilterModel(this);
    _validateModel->setDynamicSortFilter(true);
    _validateModel->setSortRole(Timer::StartDateTimeRole);
    _validateModel->setTimerState(Timer::Enabled);
    _validateModel->setTimeFilter(true);
    _validateModel->sort(0);
}

RecorderTimersEditor::~RecorderTimersEditor()
{
    delete ui;
}

void RecorderTimersEditor::changeEvent(QEvent *e)
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

void RecorderTimersEditor::edit(Timer *item)
{
    if (item == 0)
        return;

    _currentTimer = item;

    ui->checkBoxEnabled->setChecked(_currentTimer->state() != Timer::Disabled);
    ui->editName->setText(_currentTimer->name());
    ui->editChannel->setText(_currentTimer->channel());
    ui->editType->setCurrentIndex(_currentTimer->type());
    ui->editDate->setDate(_currentTimer->date());
    ui->editStartTime->setTime(_currentTimer->startTime());
    ui->editEndTime->setTime(_currentTimer->endTime());
}

bool RecorderTimersEditor::save()
{
    if (ui->checkBoxEnabled->isChecked() && !validate())
        return false;

    _currentTimer->setName(ui->editName->text());
    _currentTimer->setType(Timer::Type(ui->editType->currentIndex()));
    _currentTimer->setDate(ui->editDate->date());
    _currentTimer->setStartTime(ui->editStartTime->time());
    _currentTimer->setEndTime(ui->editEndTime->time());
    _currentTimer->setState(ui->checkBoxEnabled->isChecked() ? Timer::Enabled : Timer::Disabled);

    return true;
}

void RecorderTimersEditor::setModel(TimersModel *model)
{
    _modelCore = model;
    _validateModel->setSourceModel(model);
}

bool RecorderTimersEditor::validate()
{
    if (ui->editDate->date() < QDate::currentDate()) {
        QMessageBox::warning(this, tr("Recorder"),
                    tr("The recording start date is before today.\nYour changes will not be saved."));
        return false;
    }

    _validateModel->setStartTime(QDateTime(ui->editDate->date(), ui->editStartTime->time()));
    if (ui->editStartTime->time() > ui->editEndTime->time())
        _validateModel->setEndTime(QDateTime(ui->editDate->date().addDays(1), ui->editEndTime->time()));
    else
        _validateModel->setEndTime(QDateTime(ui->editDate->date(), ui->editEndTime->time()));

    if (_validateModel->rowCount() == 0) {
        return true;
    } else if (_validateModel->rowCount() == 1) {
        if (_modelCore->row(_validateModel->mapToSource(_validateModel->index(0, 0)).row()) == _currentTimer) {
            return true;
        } else {
            QMessageBox::warning(this, tr("Recorder"),
                        tr("The recording is overlaping with others.\nYour changes will not be saved."));
            return false;
        }
    } else {
        QMessageBox::warning(this, tr("Recorder"),
                    tr("The recording is overlaping with others.\nYour changes will not be saved."));
        return false;
    }
}
