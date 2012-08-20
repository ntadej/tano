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

#include "PlaylistFilterWidget.h"
#include "ui_PlaylistFilterWidget.h"

PlaylistFilterWidget::PlaylistFilterWidget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::PlaylistFilterWidget)
{
    ui->setupUi(this);

    connect(ui->comboCategory, SIGNAL(currentIndexChanged(QString)), this, SLOT(processFilters()));
    connect(ui->comboLanguage, SIGNAL(currentIndexChanged(QString)), this, SLOT(processFilters()));
    connect(ui->comboType, SIGNAL(currentIndexChanged(QString)), this, SLOT(processFilters()));
    connect(ui->editSearch, SIGNAL(textChanged(QString)), this, SLOT(processFilters()));
}

PlaylistFilterWidget::~PlaylistFilterWidget()
{
    delete ui;
}

void PlaylistFilterWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        ui->comboCategory->setItemText(0, tr("All categories"));
        ui->comboLanguage->setItemText(0, tr("All languages"));
        processFilters();
        break;
    default:
        break;
    }
}

void PlaylistFilterWidget::editMode()
{
    ui->comboCategory->hide();
    ui->comboLanguage->hide();

    ui->editSearch->show();
    ui->buttonClear->show();
}

void PlaylistFilterWidget::processFilters()
{
    QList<Channel::Type> list;
    switch (ui->comboType->currentIndex())
    {
    case 1:
        list << Channel::SD
             << Channel::HD;
        break;
    case 2:
        list << Channel::SD;
        break;
    case 3:
        list << Channel::HD;
        break;
    case 4:
        list << Channel::Radio;
        break;
    case 0:
    default:
        list << Channel::SD
             << Channel::HD
             << Channel::Radio;
        break;
    }

    emit filters(ui->editSearch->text(),
                 ui->comboCategory->currentText(),
                 ui->comboLanguage->currentText(),
                 list);
}

void PlaylistFilterWidget::refreshModel(const QStringList &categories,
                                        const QStringList &languages)
{
    ui->comboCategory->clear();
    ui->comboCategory->insertItem(0, tr("All categories"));
    ui->comboCategory->insertItems(1, categories);

    ui->comboLanguage->clear();
    ui->comboLanguage->insertItem(0, tr("All languages"));
    ui->comboLanguage->insertItems(1, languages);
}
