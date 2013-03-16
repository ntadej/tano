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

#include "core/network/NetworkDownload.h"
#include "core/xmltv/containers/XmltvProgramme.h"
#include "core/xmltv/models/XmltvCrewFilterModel.h"
#include "core/xmltv/models/XmltvCrewModel.h"

#include "common/OsdFloat.h"

#include "EpgShow.h"
#include "ui_EpgShow.h"

EpgShow::EpgShow(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::EpgShow),
    _current(0),
    _image(new NetworkDownload()),
    _fullscreen(false),
    _osd(0)
{
    ui->setupUi(this);

    _crew = new XmltvCrewFilterModel(this);
    _crew->setDynamicSortFilter(true);
    ui->crew->setModel(_crew);

    ui->labelChannelIcon->setPixmap(QIcon::fromTheme("video-x-generic").pixmap(22));
    ui->labelDescriptionIcon->setPixmap(QIcon::fromTheme("text-x-generic").pixmap(22));
    ui->labelCrewIcon->setPixmap(QIcon::fromTheme("config-users").pixmap(22));
    ui->labelInfoIcon->setPixmap(QIcon::fromTheme("dialog-information").pixmap(22));
    ui->labelTimeIcon->setPixmap(QIcon::fromTheme("time-admin").pixmap(22));

    ui->buttonClose->hide();
    ui->labelBlank->hide();

    connect(_image, SIGNAL(file(QFile *)), this, SLOT(image(QFile *)));

    connect(ui->buttonPrevious, SIGNAL(clicked()), this, SLOT(previous()));
    connect(ui->buttonNext, SIGNAL(clicked()), this, SLOT(next()));
    connect(ui->buttonRecord, SIGNAL(clicked()), this, SLOT(record()));

    connect(ui->comboCrewType, SIGNAL(currentIndexChanged(int)), this, SLOT(processFilters(int)));

    connect(ui->buttonClose, SIGNAL(clicked()), this, SLOT(closeOsd()));
}

EpgShow::~EpgShow()
{
    delete ui;
    if (_current)
        delete _current;
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

void EpgShow::closeOsd()
{
    if (_fullscreen)
        _osd->floatHide();
}

void EpgShow::display(XmltvProgramme *programme)
{
    if (_current)
        delete _current;
    _current = programme;

    setWindowTitle(programme->channelDisplayName() + ": " + programme->title());
    ui->labelTitle->setText("<h1>" + programme->title() + "</h1>");
    ui->labelChannel->setText("<h3>" + programme->channelDisplayName() + "</h3>");
    ui->labelTime->setText("<h3>" + programme->start().toString("dddd, d.M.yyyy") + " (" + programme->start().toString("hh:mm") + " - " + programme->stop().toString("hh:mm") + ")</h3>");
    ui->labelInfo->setText("<h3>" + programme->categories().join(" / ") + "</h3>");
    ui->labelDescription->setText(programme->desc());
    ui->labelPhoto->setPixmap(QIcon::fromTheme("image-x-generic").pixmap(48));

    if (programme->crew()) {
        _crew->setCrewModel(programme->crew());

        ui->crew->setVisible(programme->crew()->rowCount());
        ui->labelCrew->setVisible(programme->crew()->rowCount());
        ui->labelCrewIcon->setVisible(programme->crew()->rowCount());
        ui->comboCrewType->setVisible(programme->crew()->rowCount());
    }

    _image->getFile(programme->icon());

    if (_fullscreen) {
        _osd->floatShow();
    } else {
        if (isVisible())
            activateWindow();
        else
            show();
    }
}

void EpgShow::image(QFile *file)
{
    ui->labelPhoto->setPixmap(QPixmap(file->fileName()));
    delete file;
}

void EpgShow::next()
{
    emit requestNext(_current->id(), _current->channel());
}

void EpgShow::previous()
{
    emit requestPrevious(_current->id(), _current->channel());
}

void EpgShow::processFilters(const int &type)
{
    _crew->setType(XmltvCrewMember::Type(type));
}

void EpgShow::record()
{
    emit requestRecord(_current->id());
}

void EpgShow::setFullscreen(const bool &enabled,
                            OsdFloat *widget)
{
    ui->buttonClose->setVisible(enabled);
    ui->labelBlank->setVisible(enabled);

    _fullscreen = enabled;

    if (enabled && widget) {
        _osd = widget;
        _osd->setWidget(ui->main);
    } else {
        _osd = 0;
        ui->layoutMain->addWidget(ui->main);
    }
}
