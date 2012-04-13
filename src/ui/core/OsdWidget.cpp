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

#include <QtGui/QDesktopWidget>

#ifdef Q_WS_X11
 #include <QX11Info>
#endif

#include "OsdWidget.h"
#include "ui_OsdWidget.h"

OsdWidget::OsdWidget(QWidget *parent)
    : QFrame(parent),
      ui(new Ui::OsdWidget),
      _float(false)
{
    ui->setupUi(this);

    ui->logo->hide();
    ui->blank->hide();
    ui->teletext->hide();

    _slowHideTimer = new QTimer(this);
    _slowShowTimer = new QTimer(this);

    connect(_slowHideTimer, SIGNAL(timeout()), this, SLOT(slowHide()));
    connect(_slowShowTimer, SIGNAL(timeout()), this, SLOT(slowShow()));
    connect(ui->info, SIGNAL(open(QString)), this, SIGNAL(openLink(QString)));
    connect(ui->teletext, SIGNAL(valueChanged(int)), this, SIGNAL(teletextPage(int)));

    connect(ui->buttonBack, SIGNAL(clicked()), this, SIGNAL(backClicked()));
    connect(ui->buttonMute, SIGNAL(clicked()), this, SIGNAL(muteClicked()));
    connect(ui->buttonNext, SIGNAL(clicked()), this, SIGNAL(nextClicked()));
    connect(ui->buttonPlay, SIGNAL(clicked()), this, SIGNAL(playClicked()));
    connect(ui->buttonRecordNow, SIGNAL(clicked()), this, SIGNAL(recordNowClicked()));
    connect(ui->buttonStop, SIGNAL(clicked()), this, SIGNAL(stopClicked()));
    connect(ui->buttonTeletext, SIGNAL(clicked()), this, SIGNAL(teletextClicked()));
}

OsdWidget::~OsdWidget()
{
    delete ui;
    delete _slowHideTimer;
    delete _slowShowTimer;
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

void OsdWidget::enableFloat()
{
    _float = true;

    _defaultHeight = height();
    _desktopWidth = QApplication::desktop()->width();
    _desktopHeight = QApplication::desktop()->height();

    setWindowFlags(Qt::ToolTip);
    if (_desktopWidth > 1366)
        resize(2*1366/3, height());
    else
        resize(2*_desktopWidth/3, height());
    move((_desktopWidth-width())/2, _desktopHeight-height());
}

void OsdWidget::floatHide()
{
#if defined(Q_WS_X11)
    if (QX11Info::isCompositingManagerRunning())
        _slowHideTimer->start(5);
    else
        hide();
#elif defined(Q_WS_WIN)
    _slowHideTimer->start(5);
#else
    hide();
#endif
}

void OsdWidget::floatShow()
{
    _slowHideTimer->stop();

    if (_desktopWidth > 1366)
        resize(2*1366/3, height());
    else
        resize(2*_desktopWidth/3, height());
    move((_desktopWidth-width())/2, _desktopHeight-height());

#if defined(Q_WS_X11)
    if (QX11Info::isCompositingManagerRunning())
        _slowShowTimer->start(5);
    else
        show();
#elif defined(Q_WS_WIN)
    _slowShowTimer->start(5);
#else
    show();
#endif
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
    if (!_float) {
        ui->volume->setMediaPlayer(player);
    }
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
}

void OsdWidget::setEpg(const QString &now,
                       const QString &next)
{
    ui->info->setChannelEpg(now, next);
}

void OsdWidget::setLogo(const QString &logo)
{
    QPixmap pixmap(logo);
    ui->logo->setPixmap(pixmap.scaledToHeight(height(), Qt::SmoothTransformation));
    ui->logo->show();
}

void OsdWidget::setPlayingState(const Vlc::State &state)
{
    switch (state)
    {
    case Vlc::Playing:
        ui->buttonPlay->setIcon(QIcon(":/icons/24x24/media-playback-pause.png"));
        ui->buttonPlay->setToolTip(tr("Pause"));
        ui->buttonPlay->setStatusTip(tr("Pause"));
        ui->buttonMute->setEnabled(true);
        ui->buttonTeletext->setEnabled(true);
        break;
    default:
        ui->buttonPlay->setIcon(QIcon(":/icons/24x24/media-playback-start.png"));
        ui->buttonPlay->setToolTip(tr("Play"));
        ui->buttonPlay->setStatusTip(tr("Play"));
        ui->buttonMute->setEnabled(false);
        ui->buttonTeletext->setEnabled(false);
    }
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

void OsdWidget::slowHide()
{
    setWindowOpacity(windowOpacity() - 0.04);

    if (windowOpacity() <= 0) {
        _slowHideTimer->stop();
    }
}

void OsdWidget::slowShow()
{
    setWindowOpacity(windowOpacity() + 0.08);

    if (windowOpacity() == 1) {
        _slowShowTimer->stop();
    }
}

void OsdWidget::teletext(const bool &enabled)
{
    ui->buttonTeletext->setChecked(enabled);
}

void OsdWidget::volumeDown()
{
    ui->volume->volumeDown();
}

VlcVolumeSlider *OsdWidget::volumeSlider()
{
    return ui->volume;
}

void OsdWidget::volumeUp()
{
    ui->volume->volumeUp();
}
