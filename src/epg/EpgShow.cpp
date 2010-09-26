/****************************************************************************
* EpgShow.cpp: EPG show information
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

#include "EpgShow.h"
#include "ui_EpgShow.h"

#include <QtCore/QDebug>

EpgShow::EpgShow(QWidget *parent)
	: QStackedWidget(parent),
	ui(new Ui::EpgShow),
	_image(new GetFile()),
	_loader(new EpgLoader()),
	_epgNext(""),
	_epgPrevious("")
{
	ui->setupUi(this);

	connect(_image, SIGNAL(image(QString)), this, SLOT(image(QString)));
	connect(_loader, SIGNAL(show(QStringList)), this, SLOT(display(QStringList)));

	connect(ui->buttonPrevious, SIGNAL(clicked()), this, SLOT(previous()));
	connect(ui->buttonNext, SIGNAL(clicked()), this, SLOT(next()));
}

EpgShow::~EpgShow()
{
	delete ui;
	delete _image;
	delete _loader;
}

void EpgShow::changeEvent(QEvent *e)
{
	QStackedWidget::changeEvent(e);
	switch (e->type()) {
		case QEvent::LanguageChange:
			ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

void EpgShow::open(const QString &url)
{
	setCurrentIndex(0);

	setWindowTitle(tr("Show info"));
	ui->labelTitle->setText("");
	ui->labelTime->setText("");
	ui->labelInfo->setText("");
	ui->labelDescription->setText("");
	ui->labelPhoto->setPixmap(QPixmap(":/icons/images/image.png"));

	show();

	_loader->getShow(processUrl(url));
}

void EpgShow::loadPlugin(const QString &plugin)
{
	_loader->loadPlugin(plugin);
}

void EpgShow::display(const QStringList &list)
{
	setWindowTitle(list[0]);
	ui->labelTitle->setText("<h1>" + list[0] + "</h1>");
	ui->labelTime->setText("<h2>" + list[3] + " (" + list[1] + list[2] + ")</h2>");
	ui->labelInfo->setText("<h3>" + list[4] + "</h3>");
	ui->labelDescription->setText(list[5]);

	_image->getFile(list[6]);

	_epgPrevious = list[7];
	_epgNext = list[8];

	setCurrentIndex(1);
}

void EpgShow::image(const QString &image)
{
	ui->labelPhoto->setPixmap(QPixmap(image));
}

void EpgShow::next()
{
	if(_epgNext.isEmpty())
		return;

	open(_epgNext);
}

void EpgShow::previous()
{
	if(_epgPrevious.isEmpty())
		return;

	open(_epgPrevious);
}

QString EpgShow::processUrl(const QString &url) const
{
	return QString(url).replace(QRegExp("day=.&amp;"), "");
}
