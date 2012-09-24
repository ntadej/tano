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
    #include <QtWidgets/QWidget>
#elif defined(Qt4)
    #include <QtGui/QWidget>
#endif

class PlaylistModel;
class RecorderCore;
class RecorderNewDialog;
class TimersTimeManager;
class TrayIcon;
class Timer;
class TimersModel;
class VlcInstance;
class XmltvProgramme;

namespace Ui
{
    class Recorder;
}

class Recorder : public QWidget
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
                    TrayIcon *icon);
    void writeTimers();

protected:
    void changeEvent(QEvent *e);
    QSize sizeHint() const;

signals:
    void play(Timer *);

public slots:
    void newTimer();
    void newTimerFromSchedule(XmltvProgramme *programme);
    void quickRecord();
    void recordStart(Timer *timer);
    void recordStop();

private slots:
    void recordingDelete(Timer *recording);
    void timerDelete(Timer *timer);
    void timerSave(Timer *timer);

private:
    Ui::Recorder *ui;

    QAction *_actionRecord;

    Timer *_currentTimer;

    QString _directory;

    RecorderCore *_core;
    RecorderNewDialog *_new;
    TimersTimeManager *_manager;
    TimersModel *_model;
    TrayIcon *_trayIcon;
};

#endif // TANO_RECORDER_H_
