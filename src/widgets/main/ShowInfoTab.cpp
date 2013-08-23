/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
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

#include "style/StyledBar.h"

#include "ShowInfoTab.h"
#include "ui_ShowInfoTab.h"

ShowInfoTab::ShowInfoTab(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::ShowInfoTab),
    _current(0),
    _image(new NetworkDownload())
{
    ui->setupUi(this);

    _crew = new XmltvCrewFilterModel(this);
    _crew->setDynamicSortFilter(true);
    ui->crew->setModel(_crew);

    ui->labelChannelIcon->setPixmap(QIcon::fromTheme("video-x-generic").pixmap(22));
    ui->labelInfoIcon->setPixmap(QIcon::fromTheme("dialog-information").pixmap(22));
    ui->labelTimeIcon->setPixmap(QIcon::fromTheme("time-admin").pixmap(22));
    ui->labelEmptyLogo->setPixmap(QIcon::fromTheme("list-remove").pixmap(32));

    _labelTitle = new QLabel(this);
    _labelTitle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _labelTitle->setAlignment(Qt::AlignCenter);
    ui->toolBarTop->insertWidget(ui->actionRecord, _labelTitle);

    ui->toolBarBottom->setProperty("topBorder", true);
    ui->toolBarBottom->insertWidget(ui->actionPrevious, new SimpleSeparator(this));
    ui->toolBarBottom->addWidget(new SimpleSeparator(this));

    connect(_image, SIGNAL(file(QFile *)), this, SLOT(image(QFile *)));

    connect(ui->actionPrevious, SIGNAL(triggered()), this, SLOT(previous()));
    connect(ui->actionNext, SIGNAL(triggered()), this, SLOT(next()));
    connect(ui->actionRecord, SIGNAL(triggered()), this, SLOT(record()));

    clear();
}

ShowInfoTab::~ShowInfoTab()
{
    delete ui;
    if (_current)
        delete _current;
    delete _image;
}

void ShowInfoTab::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ShowInfoTab::clear()
{
    ui->stackedWidget->setCurrentIndex(1);
    _labelTitle->setText(tr("No information"));

#if FEATURE_RECORDER
    ui->toolBarTop->removeAction(ui->actionRecord);
#endif
    ui->toolBarBottom->hide();
}

void ShowInfoTab::display(XmltvProgramme *programme)
{
    if (_current)
        delete _current;
    _current = programme;

    _labelTitle->setText(programme->title());
    ui->labelChannel->setText("<b>" + programme->channelDisplayName() + "</b>");
    ui->labelTime->setText("<b>" + QDateTime::fromTime_t(programme->start()).toString("dddd, d.M.yyyy") + " (" + QDateTime::fromTime_t(programme->start()).toString("hh:mm") + " - " + QDateTime::fromTime_t(programme->stop()).toString("hh:mm") + ")</b>");

    if (programme->categories().length()) {
        ui->labelInfo->setText("<b>" + programme->categories().join(" / ") + "</b>");
        ui->labelInfo->show();
        ui->labelInfoIcon->show();
    } else {
        ui->labelInfo->hide();
        ui->labelInfoIcon->hide();
    }

    ui->labelDescription->setText(programme->desc());

    if (programme->icon().isEmpty())
        ui->labelPhoto->hide();

    if (programme->crew()) {
        _crew->setCrewModel(programme->crew());
        _crew->setType();
        ui->crew->setVisible(programme->crew()->rowCount());
    }

    _image->getFile(programme->icon());

    ui->stackedWidget->setCurrentIndex(0);
#if FEATURE_RECORDER
    ui->toolBarTop->addAction(ui->actionRecord);
#endif
    ui->toolBarBottom->show();

    emit changeTo(this);
}

void ShowInfoTab::image(QFile *file)
{
    QPixmap pixmap(file->fileName());
    ui->labelPhoto->show();
    ui->labelPhoto->setPixmap(pixmap);//.scaledToHeight(100, Qt::SmoothTransformation));
    delete file;
}

void ShowInfoTab::next()
{
    emit requestNext(_current->id(), _current->channel());
}

void ShowInfoTab::previous()
{
    emit requestPrevious(_current->id(), _current->channel());
}

void ShowInfoTab::record()
{
    emit requestRecord(_current->id());
}
