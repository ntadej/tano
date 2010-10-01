/****************************************************************************
* About.cpp: About dialog for Tano application
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

#include "About.h"
#include "ui_About.h"

#include <QtCore/QDate>

#include "Config.h"
#include "core/Version.h"

About::About(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::About)
{
	ui->setupUi(this);

	ui->labelVersion->setText("<h2>"+Version::version()+"</h2>");
	ui->labelChangeset->setText("<h2>"+Version::changeset()+"</h2>");
	ui->labelCopy->setText(ui->labelCopy->text().replace("%1",QDate().currentDate().toString("yyyy")));
	ui->labelLicense->setText(ui->labelLicense->text().replace("GNU GPL","<a href='#'>GNU GPL</a>"));
	ui->labelLinks->setText(ui->labelLinks->text().replace("http://tanoplayer.co.cc","<a href='http://tanoplayer.co.cc'>http://tanoplayer.co.cc</a>"));
	ui->labelLinks->setText(ui->labelLinks->text().replace("info@tanoplayer.co.cc","<a href='info@tanoplayer.co.cc'>info@tanoplayer.co.cc</a>"));

	ui->labelQVlc->setText("<h3>libvlc-qt "+Version::libVLCQt()+"</h3>");
	ui->labelVlc->setText("<h3>libvlc "+Version::libVLC().split(" ")[0]+"</h3>");
	ui->labelQt->setText("<h3>Qt "+Version::Qt()+"</h3>");

#if DESKTOP
	ui->labelExtra->setText("<h3>Desktop Edition</h3>");
#endif
#if DEBUG_CONSOLE
	ui->labelExtra->setText("<h3>Debug Edition</h3>");
#endif

	connect(ui->buttonLicense, SIGNAL(clicked()), this, SLOT(license()));
	connect(ui->labelLicense, SIGNAL(linkActivated(QString)), this, SLOT(license()));
}

About::~About()
{
	delete ui;
}

void About::changeEvent(QEvent *e)
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

void About::license()
{
	if(ui->stackedWidget->currentIndex() == 1)
		ui->stackedWidget->setCurrentIndex(0);
	else
		ui->stackedWidget->setCurrentIndex(1);
}
