/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_TIMERSEDIT_H_
#define TANO_TIMERSEDIT_H_

#include <QtCore/QDate>
#include <QtGui/QMainWindow>

class Channel;
class PlaylistModel;
class Timer;
class TimersModel;

namespace Ui
{
    class TimersEdit;
}

class TimersEdit : public QMainWindow
{
Q_OBJECT
public:
    TimersEdit(PlaylistModel *playlist,
               QWidget *parent = 0);
    ~TimersEdit();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);

private slots:
    void addItem();
    void deleteItem();
    void newItem();

    void edit(Timer *item);
    void playlist(Channel *channel);

    void validate();

    void editName(const QString &name);
    void editType(const int &type);
    void editDate(const QDate &date);
    void editStartTime(const QTime &time);
    void editEndTime(const QTime &time);

    void read();
    void write();
    void exit();

signals:
    void updateTimers();

private:
    void createConnections();
    void createSettings();

    Ui::TimersEdit *ui;

    Channel *_channel;

    bool _closeEnabled;

    TimersModel *_timersModel;
    PlaylistModel *_playlistModel;

    QString _path;
    QString _playlist;
};

#endif // TANO_TIMERSEDIT_H_
