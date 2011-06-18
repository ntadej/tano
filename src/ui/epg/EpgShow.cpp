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

#include "container/xmltv/XmltvProgramme.h"
#include "core/GetFile.h"

#include "EpgShow.h"
#include "ui_EpgShow.h"

EpgShow::EpgShow(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::EpgShow),
    _image(new GetFile())
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
}

void EpgShow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void EpgShow::display(XmltvProgramme *programme)
{
    _current = programme;

    setWindowTitle(programme->channelDisplayName() + ": " + programme->title());
    ui->labelTitle->setText("<h1>" + programme->title() + "</h1>");
    ui->labelTime->setText("<h2>" + programme->channelDisplayName()+ ", " + programme->start().toString("dddd, d.M.yyyy") + " (" + programme->start().toString("hh:mm") + " - " + programme->stop().toString("hh:mm") + ")</h2>");
    QString categories;
    for(int i = 0; i < programme->category().size(); i++) {
        if(i != 0)
            categories.append(" / ");
        categories.append(programme->category()[i]);
    }
    ui->labelInfo->setText("<h3>" + categories + "</h3>");
    ui->labelDescription->setText(programme->desc());
    ui->labelPhoto->setPixmap(QPixmap(":/icons/48x48/image.png"));

    _image->getFile(programme->icon());

    show();
}

void EpgShow::image(const QString &image)
{
    ui->labelPhoto->setPixmap(QPixmap(image));
}

void EpgShow::next()
{
    emit requestNext(_current);
}

void EpgShow::previous()
{
    emit requestPrevious(_current);
}
