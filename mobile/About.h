/****************************************************************************
* About.h: About dialog (Tano Mobile)
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

#ifndef TANOMOBILE_ABOUT_H_
#define TANOMOBILE_ABOUT_H_

#include <QtGui/QDialog>

namespace Ui {
	class About;
}

class About : public QDialog
{
Q_OBJECT

public:
	About(const QString &version, QWidget *parent = 0);
	About(QWidget *parent = 0);
	~About();

protected:
	void changeEvent(QEvent *e);

private:
	Ui::About *ui;
};

#endif // TANOMOBILE_ABOUT_H_
