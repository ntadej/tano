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

#include "core/Common.h"
#include "core/Settings.h"

#include "ConclusionPage.h"
#include "ui_ConclusionPage.h"

ConclusionPage::ConclusionPage(QWidget *parent)
    : QWizardPage(parent),
    ui(new Ui::ConclusionPage)
{
    ui->setupUi(this);

    registerField("agree*", ui->agreeCheckBox);
}

ConclusionPage::~ConclusionPage()
{
    delete ui;
}

void ConclusionPage::changeEvent(QEvent *e)
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

int ConclusionPage::nextId() const
{
    QScopedPointer<Settings> settings(new Settings());
    settings->setConfigurationVersion(Tano::version());
    settings->setConfigured(true);
    settings->setPlaylist(field("playlist").toString());
    settings->setSessionVolume(field("sessionvolume").toBool());
    settings->setSessionAutoplay(field("sessionplay").toBool());

    settings->writeSettings();

    return -1;
}
