/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "container/EpgShowInfo.h"
#include "core/GetFile.h"
#include "epg/EpgSloveniaLoader.h"

#include "EpgShow.h"
#include "ui_EpgShow.h"

EpgShow::EpgShow(QWidget *parent)
	: QStackedWidget(parent),
	ui(new Ui::EpgShow),
	_image(new GetFile()),
	_slovenia(new EpgSloveniaLoader()),
	_epgNext(""),
	_epgPrevious("")
{
	ui->setupUi(this);

	connect(_image, SIGNAL(file(QString)), this, SLOT(image(QString)));

	connect(ui->buttonPrevious, SIGNAL(clicked()), this, SLOT(previous()));
	connect(ui->buttonNext, SIGNAL(clicked()), this, SLOT(next()));
}

EpgShow::~EpgShow()
{
	delete ui;
	delete _image;
	delete _slovenia;
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

void EpgShow::get(const QString &id)
{
	setCurrentIndex(0);

	setWindowTitle(tr("Show info"));
	ui->labelTitle->setText("");
	ui->labelTime->setText("");
	ui->labelInfo->setText("");
	ui->labelDescription->setText("");
	ui->labelPhoto->setPixmap(QPixmap(":/icons/48x48/image.png"));

	show();

	if(_type == Tano::Slovenia)
		_slovenia->getShowInfo(processUrl(id));
}

void EpgShow::setEpgType(const Tano::EpgType type)
{
	disconnect(_slovenia, SIGNAL(showInfo(EpgShowInfo)), this, SLOT(display(EpgShowInfo)));

	_type = type;

	if(_type == Tano::Slovenia) {
		connect(_slovenia, SIGNAL(showInfo(EpgShowInfo)), this, SLOT(display(EpgShowInfo)));
	}
}

void EpgShow::display(const EpgShowInfo &info)
{
	setWindowTitle(info.title());
	ui->labelTitle->setText("<h1>" + info.title() + "</h1>");
	ui->labelTime->setText("<h2>" + info.startTime().toString("dddd, d.M.yyyy") + " (" + info.startTime().toString("hh:mm") + " - " + info.endTime().toString("hh:mm") + ")</h2>");
	ui->labelInfo->setText("<h3>" + info.info() + "</h3>");
	ui->labelDescription->setText(info.description());

	_image->getFile(info.image());

	_epgPrevious = info.previous();
	_epgNext = info.next();

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

	get(_epgNext);
}

void EpgShow::previous()
{
	if(_epgPrevious.isEmpty())
		return;

	get(_epgPrevious);
}

QString EpgShow::processUrl(const QString &url) const
{
	return QString(url).replace(QRegExp("day=.&amp;"), "");
}
