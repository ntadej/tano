/****************************************************************************
* AboutDialog.cpp: About dialog for Tano application
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

#include "AboutDialog.h"
#include "ui_AboutDialog.h"

#include <QtCore/QDate>

#include "core/Version.h"

AboutDialog::AboutDialog(QWidget *parent)
	: QDialog(parent),
	ui(new Ui::AboutDialog)
{
	ui->setupUi(this);

	ui->labelVersion->setText("<h2>"+Version::version()+"</h2>");
	ui->labelChangeset->setText("<h2>"+Version::changeset()+"</h2>");
	ui->labelCopy->setText(ui->labelCopy->text().replace("%1",QDate().currentDate().toString("yyyy")));
	ui->labelLicense->setText(ui->labelLicense->text().replace("GNU GPL","<a href='#'>GNU GPL</a>"));
	ui->labelLinks->setText(ui->labelLinks->text().replace("http://tanoplayer.co.cc","<a href='http://tanoplayer.co.cc'>http://tanoplayer.co.cc</a>"));
	ui->labelLinks->setText(ui->labelLinks->text().replace("info@tanoplayer.co.cc","<a href='info@tanoplayer.co.cc'>info@tanoplayer.co.cc</a>"));

	ui->labelQVlc->setText("<h3>libvlc-qt "+Version::libVlcqt()+"</h3>");
	ui->labelVlc->setText("<h3>libvlc "+Version::libVlc().split(" ")[0]+"</h3>");
	ui->labelQt->setText("<h3>Qt "+Version::qt()+"</h3>");

	connect(ui->buttonLicense, SIGNAL(clicked()), this, SLOT(license()));
	connect(ui->labelLicense, SIGNAL(linkActivated(QString)), this, SLOT(license()));
}

AboutDialog::~AboutDialog()
{
	delete ui;
}

void AboutDialog::changeEvent(QEvent *e)
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

void AboutDialog::license()
{
	if(ui->stackedWidget->currentIndex() == 1)
		ui->stackedWidget->setCurrentIndex(0);
	else
		ui->stackedWidget->setCurrentIndex(1);
}
