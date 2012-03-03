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

#ifndef TANO_RECORDERCORE_H_
#define TANO_RECORDERCORE_H_

#include <QtCore/QObject>
#include <QtCore/QTimer>

// VLC-Qt
class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;

class RecorderTimeManager;
class Timer;

class RecorderCore : public QObject
{
Q_OBJECT
public:
    RecorderCore(QObject *parent = 0);
    ~RecorderCore();

    bool isRecording() const { return _isRecording; }
    bool isTimer() const { return _isTimer; }
    QString output() const;
    void settings();
    void stop();
    QString timerEndTime() const { return _currentEndTime; }
    void timerInfo();
    void updateTimers();

public slots:
    void record(const QString &channel,
                const QString &url,
                const QString &path);
    void record(Timer *t);

signals:
    void elapsed(const int &);
    void timer(const QString &,
               const QString &);
    void timerStop();

private slots:
    void time();

private:
    void recordBackend(const QString &url);
    QString fileName(const QString &channel,
                     const QString &name = 0) const;

    QString _currentEndTime;
    QString _currentName;
    QString _currentUrl;
    QString _defaultPath;

    bool _isRecording;
    bool _isTimer;

    QString _output;
    QString _outputName;
    QString _outputPath;

    VlcInstance *_instance;
    VlcMedia *_media;
    VlcMediaPlayer *_player;

    int _time;
    QTimer *_timer;

    RecorderTimeManager *_manager;
};

#endif // TANO_RECORDERCORE_H_
