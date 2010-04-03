/****************************************************************************
* IntroPage.h: First run wizard - Intro page
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

#ifndef TANO_INTROPAGE_H_
#define TANO_INTROPAGE_H_

#include <QtGui/QWizardPage>

namespace Ui
{
	class IntroPage;
}

class IntroPage : public QWizardPage
{
Q_OBJECT
public:
	IntroPage(QWidget *parent = 0);
	~IntroPage();

	int nextId() const;

protected:
	void changeEvent(QEvent *e);

private:
	Ui::IntroPage *ui;
};

#endif // TANO_INTROPAGE_H_
