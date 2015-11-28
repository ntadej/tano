/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
*
* The UI layout was based on the VLMC About dialog
* Copyright (C) 2008-2010 VideoLAN
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

#include <QtCore/QDate>

#include "common/Common.h"
#include "plugins/Plugins.h"

#include "common/Backend.h"

#include "AboutDialog.h"
#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    ui->labelIcon->setPixmap(QIcon(":/logo/64x64/logo.png").pixmap(64));
    ui->labelIconBackend->setPixmap(QIcon(":/logo/24x24/vlc-qt.png").pixmap(24));

    ui->labelTitle->setText(ui->labelTitle->text().arg(Tano::name(), Tano::version(), Tano::changeset(), Tano::is64bit() ? "64-bit" : "32-bit", globalConfig ? globalConfig->projectUrl() : "http://projects.tano.si"));
    ui->labelTano->setText(ui->labelTano->text().arg(Tano::versionCore()));
    ui->labelBackend->setText(ui->labelBackend->text().arg(Tano::Backend::versionLibrary()));
    ui->labelCopyright->setText(ui->labelCopyright->text().arg(QDate::currentDate().toString("yyyy")));

    if (Tano::name() == "Tano") {
        ui->labelTano->hide();
    }
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::changeEvent(QEvent *e)
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
