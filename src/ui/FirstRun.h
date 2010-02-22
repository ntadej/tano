/****************************************************************************
* FirstRun.h: First run wizard
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

#ifndef TANO_FIRSTRUN_H_
#define TANO_FIRSTRUN_H_

#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>
#include <QtGui/QWizard>
#include "License.h"

class FirstRun : public QWizard
{
Q_OBJECT

public:
	enum { Page_Intro, Page_Settings, Page_Conclusion };

	FirstRun(QWidget *parent = 0);
	~FirstRun();
};


class IntroPage : public QWizardPage
{
Q_OBJECT

public:
	IntroPage(QWidget *parent = 0);
	~IntroPage();

	int nextId() const;

private:
	QLabel *_topLabel;
	QLabel *_versionLabel;
	QSettings *_settings;
};


class SettingsPage : public QWizardPage
{
Q_OBJECT

public:
	SettingsPage(QWidget *parent = 0);
	~SettingsPage();

	int nextId() const;

private slots:
	void setPlaylist();

private:
	QLabel *_settingsLabel;
	QLabel *_spacerLabel;
	QCheckBox *_sessionBox;
	QCheckBox *_vlcBox;
	QRadioButton *_siolRadio2;
	QRadioButton *_siolRadio4;
	QRadioButton *_T2Radio;
	QRadioButton *_tusRadio;
	QLineEdit *_playlist;
	QLineEdit *_type;
};


class ConclusionPage : public QWizardPage
{
Q_OBJECT

public:
	ConclusionPage(QWidget *parent = 0);
	~ConclusionPage();

	int nextId() const;

private:
	QLabel *_topLabel;
	QCheckBox *_agreeCheckBox;
	License *_license;
	QSettings *_settings;
};

#endif // TANO_FIRSTRUN_H_
