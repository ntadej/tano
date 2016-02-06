/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
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

#include "application/Common.h"

#include "ChannelsEditorHelp.h"
#include "ui_ChannelsEditorHelp.h"

ChannelsEditorHelp::ChannelsEditorHelp(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::ChannelsEditorHelp)
{
    ui->setupUi(this);

    setWindowTitle(windowTitle().replace("Tano", Tano::name()));
    ui->labelInfo->setText(ui->labelInfo->text().replace("Tano", Tano::name()));
}

ChannelsEditorHelp::~ChannelsEditorHelp()
{
    delete ui;
}

void ChannelsEditorHelp::changeEvent(QEvent *e)
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
