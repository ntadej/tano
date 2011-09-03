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

#include "core/LocaleManager.h"

#include "ConclusionPage.h"
#include "FirstRunWizard.h"
#include "IntroPage.h"
#include "SettingsPage.h"

FirstRunWizard::FirstRunWizard(QWidget *parent)
    : QWizard(parent),
    _locale(new LocaleManager())
{
    setPage(Intro, new IntroPage);
    setPage(Settings, new SettingsPage);
    setPage(Conclusion, new ConclusionPage);

    setStartId(Intro);

#ifndef Q_WS_MAC
    setWizardStyle(ModernStyle);
#endif

    setPixmap(QWizard::LogoPixmap, QPixmap(":/logo/48x48/logo.png"));
    setWindowTitle(tr("First Run Wizard"));
}

FirstRunWizard::~FirstRunWizard()
{
    delete _locale;
}
