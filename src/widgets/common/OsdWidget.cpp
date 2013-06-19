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
#include "core/platform/Features.h"

#include "common/InfoBarWidget.h"

#include "OsdWidget.h"
#include "ui_OsdWidget.h"

OsdWidget::OsdWidget(QWidget *parent)
    : StyledBar(parent),
      ui(new Ui::OsdWidget),
      _current(""),
      _file(new NetworkDownload())
{
    ui->setupUi(this);

    setLightColored(true);
    setSingleRow(false);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    _info = new InfoBarWidget(this);
    ui->controls->addWidget(_info);

    ui->logo->hide();
    ui->teletext->hide();

    ui->buttonBack->setProperty("extraframe", true);
    ui->buttonMute->setProperty("extraframe", true);
    ui->buttonNext->setProperty("extraframe", true);
    ui->buttonPlay->setProperty("extraframe", true);
    ui->buttonRecordNow->setProperty("extraframe", true);
    ui->buttonSnapshot->setProperty("extraframe", true);
    ui->buttonStop->setProperty("extraframe", true);
    ui->buttonTeletext->setProperty("extraframe", true);

    connect(_info, SIGNAL(open(QString)), this, SIGNAL(openLink(QString)));
    connect(ui->teletext, SIGNAL(valueChanged(int)), this, SIGNAL(teletextPage(int)));

    connect(ui->buttonBack, SIGNAL(clicked()), this, SIGNAL(backClicked()));
    connect(ui->buttonMute, SIGNAL(clicked()), this, SIGNAL(muteClicked()));
    connect(ui->buttonNext, SIGNAL(clicked()), this, SIGNAL(nextClicked()));
    connect(ui->buttonPlay, SIGNAL(clicked()), this, SIGNAL(playClicked()));
    connect(ui->buttonRecordNow, SIGNAL(clicked()), this, SIGNAL(recordNowClicked()));
    connect(ui->buttonSnapshot, SIGNAL(clicked()), this, SIGNAL(snapshotClicked()));
    connect(ui->buttonStop, SIGNAL(clicked()), this, SIGNAL(stopClicked()));
    connect(ui->buttonTeletext, SIGNAL(clicked()), this, SIGNAL(teletextClicked()));

    connect(_file, SIGNAL(file(QFile *)), this, SLOT(setLogo(QFile *)));

#if !FEATURE_RECORDER
    ui->buttonRecordNow->hide();
    ui->buttonSnapshot->hide();
#endif
}

OsdWidget::~OsdWidget()
{
    delete ui;
}

void OsdWidget::changeEvent(QEvent *e)
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

QLCDNumber *OsdWidget::lcd()
{
    return ui->number;
}

void OsdWidget::mute(const bool &enabled)
{
    ui->volume->setMute(enabled);
    ui->buttonMute->setChecked(enabled);
}

void OsdWidget::setBackend(VlcMediaPlayer *player)
{
    ui->volume->setMediaPlayer(player);
    ui->seek->setMediaPlayer(player);
    ui->seek->setAutoHide(true);
}

void OsdWidget::setChannel(const int &number,
                           const QString &name,
                           const QString &language)
{
    if (number) {
        _info->setChannelInfo(name, language);
        ui->number->display(number);
    } else {
        _info->clear();
        ui->logo->hide();
    }

    _current = "";
}

void OsdWidget::setEpg(const QStringList &epg)
{
    if (epg.size() < 2)
        return;

    _info->setChannelEpg(epg[0], epg[1]);

    QRegExp n("href=\"([^\"]*)");
    n.indexIn(epg[0]);
    _current = n.cap(1);
}

void OsdWidget::setLogo(const QString &file)
{
    if (file.contains("http")) {
        _file->getFile(file);
    } else if (!file.isEmpty()) {
        QPixmap pixmap(file);
        ui->logo->setPixmap(pixmap.scaledToHeight(height(), Qt::SmoothTransformation));
        ui->logo->show();
    }
}

void OsdWidget::setLogo(QFile *file)
{
    setLogo(file->fileName());
    delete file;
}

void OsdWidget::setPlaying(const bool &playing)
{
    if (playing) {
        ui->buttonPlay->setIcon(QIcon::fromTheme("media-playback-pause"));
        ui->buttonPlay->setToolTip(tr("Pause"));
        ui->buttonPlay->setStatusTip(tr("Pause"));
        ui->buttonMute->setEnabled(true);
        ui->buttonTeletext->setEnabled(true);
    } else {
        ui->buttonPlay->setIcon(QIcon::fromTheme("media-playback-start"));
        ui->buttonPlay->setToolTip(tr("Play"));
        ui->buttonPlay->setStatusTip(tr("Play"));
        ui->buttonMute->setEnabled(false);
        ui->buttonTeletext->setEnabled(false);
    }
}

void OsdWidget::setQuickRecordChecked(const bool &enabled)
{
    ui->buttonRecordNow->setChecked(enabled);
}

void OsdWidget::setRecording(const QString &name,
                             const QString &info)
{
    if (!name.isEmpty()) {
        _info->setRecordingInfo(name, info);
    } else {
        _info->clear();
    }
}

void OsdWidget::setTeletextPage(const int &page)
{
    if (page != ui->teletext->value())
        ui->teletext->setValue(page);
}

void OsdWidget::setVideoState(const bool &enabled)
{
    ui->buttonSnapshot->setEnabled(enabled);
    ui->buttonRecordNow->setEnabled(enabled);
    ui->buttonRecordNow->setChecked(false);
}

void OsdWidget::teletext(const bool &enabled)
{
    ui->buttonTeletext->setChecked(enabled);
}

void OsdWidget::toggleTeletext(const bool &enabled)
{
    ui->buttonTeletext->setVisible(enabled);
}

void OsdWidget::volumeDown()
{
    ui->volume->volumeDown();
}

VolumeSlider *OsdWidget::volumeSlider()
{
    return ui->volume;
}

void OsdWidget::volumeUp()
{
    ui->volume->volumeUp();
}
