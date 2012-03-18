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
#include <QtGui/QStackedWidget>

class Channel;
class PlaylistModel;
class RecorderCore;
class RecorderInfoWidget;
class RecorderTimeManager;
class TrayIcon;
class TimersEditor;
class TimersModel;
class Udpxy;

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
    bool isRecording() const;
    void refreshPlaylistModel();
    void setPlaylistModel(PlaylistModel *model);
    void setWidgets(QAction *action,
                    RecorderInfoWidget *info,
                    TrayIcon *icon);

protected:
    void changeEvent(QEvent *e);

public slots:
    void quickRecord();
    void recordNow(const QString &name,
                   const QString &url);
    void recordStart();
    void recordStop();
    void showTimersEditor();

private slots:
    void playlist(Channel* channel);
    void quickRecordCancel();
    void quickRecordStart();
    void timerStart(const QString &name,
                    const QString &url);
    void timerStop();

private:
    Ui::Recorder *ui;

    QAction *_actionRecord;

    QString _directory;
    QString _name;
    QString _url;

    Channel *_currentChannel;

    RecorderCore *_core;
    TimersEditor *_editor;
    RecorderInfoWidget *_info;
    RecorderTimeManager *_manager;
    TimersModel *_model;
    PlaylistModel *_playlistModel;
    TrayIcon *_trayIcon;
    Udpxy *_udpxy;
};

#endif // TANO_RECORDER_H_
