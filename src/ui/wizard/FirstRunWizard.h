/****************************************************************************
* FirstRunWizard.h: First run wizard
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

#ifndef TANO_FIRSTRUNWIZARD_H_
#define TANO_FIRSTRUNWIZARD_H_

#include <QtGui/QWizard>

#include "core/LocaleManager.h"

class FirstRunWizard : public QWizard
{
Q_OBJECT
public:
	enum { Intro, Settings, Playlist, Conclusion };

	FirstRunWizard(QWidget *parent = 0);
	~FirstRunWizard();

private:
	LocaleManager *_locale;
};

#endif // TANO_FIRSTRUNWIZARD_H_
