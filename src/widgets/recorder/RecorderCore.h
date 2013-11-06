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

#include <QtCore/QDate>
#include <QtCore/QObject>
#include <QtCore/QTime>
#include <QtCore/QTimer>

// VLC-Qt
class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;

class Timer;

class RecorderCore : public QObject
{
Q_OBJECT
public:
    RecorderCore(QObject *parent = 0);
    ~RecorderCore();

    bool isRecording() const { return _isRecording; }
    bool isTimer() const { return _isTimer; }
    void setDefaultOutputPath(const QString &path) { _defaultPath = path; }
    void setMediaInstance(VlcInstance *instance);
    void stop();

public slots:
    void record(Timer *t,
                const QString &channelName,
                const QString &url);

signals:
    void elapsed(int );
    void timerStop();

private slots:
    void time();

private:
    bool _isRecording;
    bool _isTimer;

    QString _defaultPath;

    QString _output;
    QString _outputName;
    QString _outputPath;
    QDateTime _currentEndTime;

    VlcInstance *_instance;
    VlcMedia *_media;
    VlcMediaPlayer *_player;

    int _time;
    QTimer *_timer;
};

#endif // TANO_RECORDERCORE_H_
