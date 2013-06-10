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

#ifndef TANO_OSDWIDGET_H_
#define TANO_OSDWIDGET_H_

#include <QtCore/QFile>

#include <vlc-qt/Enums.h>

#include "style/StyledBar.h"

namespace Ui {
    class OsdWidget;
}

class QLabel;
class QLCDNumber;

class VlcMediaPlayer;

class VlcWidgetVolumeSlider;

class OsdWidget : public StyledBar
{
Q_OBJECT
public:
    explicit OsdWidget(QWidget *parent = 0);
    ~OsdWidget();

    QWidget *blank();
    inline QString currentProgramme() const { return _current; }
    QLCDNumber *lcd();
    void setBackend(VlcMediaPlayer *player);
    void setChannel(const int &number = 0,
                    const QString &name = 0,
                    const QString &language = 0);
    void setPlaying(const bool &playing);
    void setQuickRecordChecked(const bool &enabled);
    void setRecording(const QString &name = 0,
                      const QString &info = 0);
    void setVideoState(const bool &enabled);
    void toggleTeletext(const bool &enabled);
    VlcWidgetVolumeSlider *volumeSlider();

protected:
    void changeEvent(QEvent *e);

signals:
    void openLink(const QString &);
    void teletextPage(const int &);

    void backClicked();
    void muteClicked();
    void nextClicked();
    void playClicked();
    void recordNowClicked();
    void snapshotClicked();
    void stopClicked();
    void teletextClicked();

public slots:
    void mute(const bool &enabled);
    void setEpg(const QStringList &epg);
    void setLogo(const QString &file);
    void setLogo(QFile *file);
    void setTeletextPage(const int &page);
    void teletext(const bool &enabled);
    void volumeDown();
    void volumeUp();

private:
    Ui::OsdWidget *ui;

    QString _current;
};

#endif // OSDWIDGET_H
