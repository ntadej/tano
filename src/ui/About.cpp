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

#include <QtCore/QDate>

#include "About.h"
#include "core/Version.h"

About::About(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	ui.labelVersion->setText("<h2>"+Version::Tano()+"</h2>");
	ui.labelCopy->setText(ui.labelCopy->text().replace("%1",QDate().currentDate().toString("yyyy")));
	ui.labelLicense->setText(ui.labelLicense->text().replace("GNU GPL","<a href='#'>GNU GPL</a>"));
	ui.labelLinks->setText(ui.labelLinks->text().replace("http://tanoplayer.co.cc","<a href='http://tanoplayer.co.cc'>http://tanoplayer.co.cc</a>"));
	ui.labelLinks->setText(ui.labelLinks->text().replace("info@tanoplayer.co.cc","<a href='info@tanoplayer.co.cc'>info@tanoplayer.co.cc</a>"));

	ui.labelQVlc->setText("<h3>QVlc "+Version::libQVlc()+"</h3>");
	ui.labelVlc->setText("<h3>libVLC "+Version::libVLC().split(" ")[0]+"</h3>");
	ui.labelQt->setText("<h3>Qt "+Version::Qt()+"</h3>");

	connect(ui.buttonLicense, SIGNAL(clicked()), this, SLOT(license()));
	connect(ui.labelLicense, SIGNAL(linkActivated(QString)), this, SLOT(license()));
}

About::~About()
{

}

void About::license()
{
	if(ui.stackedWidget->currentIndex() == 1)
		ui.stackedWidget->setCurrentIndex(0);
	else
		ui.stackedWidget->setCurrentIndex(1);
}
