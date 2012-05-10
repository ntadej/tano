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

#include "core/Common.h"
#include "core/Settings.h"
#include "FirstRunWizard.h"

#include "IntroPage.h"
#include "ui_IntroPage.h"

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent),
    ui(new Ui::IntroPage)
{
    ui->setupUi(this);

    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/wizard.png"));

    QScopedPointer<Settings> settings(new Settings(this));
    if(settings->configurationVersion() != Tano::version())
        ui->labelVersion->setText(tr("You previously used version %1 of <i>Tano</i>.")
                                  .arg(settings->configurationVersion()) +"<br>"+
                                  tr("Please re-set your settings."));
    else
        ui->labelVersion->clear();
}

IntroPage::~IntroPage()
{
    delete ui;
}

void IntroPage::changeEvent(QEvent *e)
{
    QWizardPage::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

int IntroPage::nextId() const
{
    return FirstRunWizard::Settings;
}
