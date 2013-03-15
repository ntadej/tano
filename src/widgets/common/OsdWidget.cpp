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

#include "OsdWidget.h"
#include "ui_OsdWidget.h"

OsdWidget::OsdWidget(QWidget *parent)
    : QFrame(parent),
      ui(new Ui::OsdWidget),
      _current("")
{
    ui->setupUi(this);

    ui->logo->hide();
    ui->blank->hide();
    ui->teletext->hide();

    connect(ui->info, SIGNAL(open(QString)), this, SIGNAL(openLink(QString)));
    connect(ui->teletext, SIGNAL(valueChanged(int)), this, SIGNAL(teletextPage(int)));

    connect(ui->buttonBack, SIGNAL(clicked()), this, SIGNAL(backClicked()));
    connect(ui->buttonMute, SIGNAL(clicked()), this, SIGNAL(muteClicked()));
    connect(ui->buttonNext, SIGNAL(clicked()), this, SIGNAL(nextClicked()));
    connect(ui->buttonPlay, SIGNAL(clicked()), this, SIGNAL(playClicked()));
    connect(ui->buttonRecordNow, SIGNAL(clicked()), this, SIGNAL(recordNowClicked()));
    connect(ui->buttonSnapshot, SIGNAL(clicked()), this, SIGNAL(snapshotClicked()));
    connect(ui->buttonStop, SIGNAL(clicked()), this, SIGNAL(stopClicked()));
    connect(ui->buttonTeletext, SIGNAL(clicked()), this, SIGNAL(teletextClicked()));
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

QWidget *OsdWidget::blank()
{
    return ui->blank;
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
        ui->info->setChannelInfo(name, language);
        ui->number->display(number);
    } else {
        ui->info->clear();
        ui->logo->hide();
    }

    _current = "";
}

void OsdWidget::setEpg(const QString &now,
                       const QString &next)
{
    ui->info->setChannelEpg(now, next);

    QRegExp n("href=\"([^\"]*)");
    n.indexIn(now);
    _current = n.cap(1);
}

void OsdWidget::setLogo(const QString &file)
{
    QPixmap pixmap(file);
    ui->logo->setPixmap(pixmap.scaledToHeight(height(), Qt::SmoothTransformation));
    ui->logo->show();
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
        ui->info->setRecordingInfo(name, info);
    } else {
        ui->info->clear();
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

VlcWidgetVolumeSlider *OsdWidget::volumeSlider()
{
    return ui->volume;
}

void OsdWidget::volumeUp()
{
    ui->volume->volumeUp();
}
