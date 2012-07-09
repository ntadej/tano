/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
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
#include "core/NetworkDownload.h"
#include "xmltv/XmltvCrewFilterModel.h"
#include "xmltv/XmltvCrewModel.h"

#include "EpgShow.h"
#include "ui_EpgShow.h"

EpgShow::EpgShow(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::EpgShow),
    _image(new NetworkDownload())
{
    ui->setupUi(this);

    _crew = new XmltvCrewFilterModel(this);
    _crew->setDynamicSortFilter(true);
    ui->crew->setModel(_crew);

    connect(_image, SIGNAL(file(QString)), this, SLOT(image(QString)));

    connect(ui->buttonPrevious, SIGNAL(clicked()), this, SLOT(previous()));
    connect(ui->buttonNext, SIGNAL(clicked()), this, SLOT(next()));
    connect(ui->buttonRecord, SIGNAL(clicked()), this, SLOT(record()));

    connect(ui->comboCrewType, SIGNAL(currentIndexChanged(int)), this, SLOT(processFilters(int)));
}

EpgShow::~EpgShow()
{
    delete ui;
    delete _image;
}

void EpgShow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
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
    ui->labelChannel->setText("<h3>" + programme->channelDisplayName() + "</h3>");
    ui->labelTime->setText("<h3>" + programme->start().toString("dddd, d.M.yyyy") + " (" + programme->start().toString("hh:mm") + " - " + programme->stop().toString("hh:mm") + ")</h3>");
    ui->labelInfo->setText("<h3>" + programme->categories().join(" / ") + "</h3>");
    ui->labelDescription->setText(programme->desc());
    ui->labelPhoto->setPixmap(QPixmap(":/icons/48x48/image.png"));

    _crew->setSourceModel(programme->crew());

    ui->crew->setVisible(programme->crew()->rowCount());
    ui->labelCrew->setVisible(programme->crew()->rowCount());
    ui->labelCrewIcon->setVisible(programme->crew()->rowCount());
    ui->comboCrewType->setVisible(programme->crew()->rowCount());

    _image->getFile(programme->icon());

    if (isVisible())
        activateWindow();
    else
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

void EpgShow::processFilters(const int &type)
{
    _crew->setType(Tano::Xmltv::CrewMemberType(type));
}

void EpgShow::record()
{
    emit requestRecord(_current);
}
