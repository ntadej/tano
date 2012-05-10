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

#ifndef TANO_RECORDER_H_
#define TANO_RECORDER_H_

#include <QtCore/QTime>

#if defined(Qt5)
    #include <QtWidgets/QStackedWidget>
#elif defined(Qt4)
    #include <QtGui/QStackedWidget>
#endif

class Channel;
class PlaylistModel;
class RecorderCore;
class RecorderInfoWidget;
class RecorderTimeManager;
class TrayIcon;
class Timer;
class TimersModel;
class Udpxy;
class VlcInstance;
class XmltvProgramme;

namespace Ui
{
    class Recorder;
}

class Recorder : public QStackedWidget
{
Q_OBJECT
public:
    explicit Recorder(QWidget *parent = 0);
    ~Recorder();

    void createSettings();
    QString directory() const { return _directory; }
    bool isRecording() const;
    Timer *newInstantTimer(const QString &channel,
                           const QString &url);
    void refreshPlaylistModel();
    void setMediaInstance(VlcInstance *instance);
    void setPlaylistModel(PlaylistModel *model);
    void setWidgets(QAction *action,
                    RecorderInfoWidget *info,
                    TrayIcon *icon);
    void writeTimers();

protected:
    void changeEvent(QEvent *e);

signals:
    void play(Timer *);

public slots:
    void newTimer();
    void newTimerFromSchedule(XmltvProgramme *programme);
    void quickRecord();
    void recordStart(Timer *timer);
    void recordStop();

private slots:
    void cancel();
    void playlist(Channel* channel);
    void processNewTimer();
    void processQuickRecord();
    void recordingDelete(Timer *recording);
    void timerDelete(Timer *timer);
    void timerSave(Timer *timer);

private:
    Ui::Recorder *ui;

    QAction *_actionRecord;

    Channel *_currentChannel;
    Timer *_currentTimer;

    QString _directory;

    RecorderCore *_core;
    RecorderInfoWidget *_info;
    RecorderTimeManager *_manager;
    TimersModel *_model;
    PlaylistModel *_playlistModel;
    TrayIcon *_trayIcon;
    Udpxy *_udpxy;
};

#endif // TANO_RECORDER_H_
