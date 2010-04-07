/****************************************************************************
* FirstRunWizard.cpp: First run wizard
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* This file may be used under the terms of the
* GNU General Public License version 3.0 as published by the
* Free Software Foundation and appearing in the file LICENSE.GPL
* included in the packaging of this file.
*****************************************************************************/

#include "FirstRunWizard.h"

#include "ConclusionPage.h"
#include "IntroPage.h"
#include "PlaylistPage.h"
#include "SettingsPage.h"

FirstRunWizard::FirstRunWizard(QWidget *parent) :
	QWizard(parent), _locale(new LocaleManager())
{	
	setPage(Intro, new IntroPage);
	setPage(Settings, new SettingsPage);
	setPage(Playlist, new PlaylistPage);
	setPage(Conclusion, new ConclusionPage);

	setStartId(Intro);

#ifndef Q_WS_MAC
	setWizardStyle(ModernStyle);
#endif

	setPixmap(QWizard::LogoPixmap, QPixmap(":/icons/images/tano-small.png"));
	setWindowTitle(tr("First Run Wizard"));
}

FirstRunWizard::~FirstRunWizard()
{
	delete _locale;
}
