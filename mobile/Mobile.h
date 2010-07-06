/****************************************************************************
* Mobile.h: Main Widget (Tano Mobile)
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

#ifndef TANOMOBILE_MOBILE_H_
#define TANOMOBILE_MOBILE_H_

#include <QtGui/QMainWindow>

#include "core/LocaleManager.h"

namespace Ui {
	class Mobile;
}

class Mobile : public QMainWindow
{
    Q_OBJECT

public:
	explicit Mobile(QWidget *parent = 0);
	~Mobile();

protected:
	void changeEvent(QEvent *e);

private slots:
	void read();

private:
	Ui::Mobile *ui;

	LocaleManager *_locale;

	QString _playlist;
};

#endif // TANOMOBILE_MOBILE_H_
