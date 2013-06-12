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

#ifndef TANO_PLAYLISTDISPLAYWIDGET_H_
#define TANO_PLAYLISTDISPLAYWIDGET_H_

#include <QtCore/QModelIndex>

#if QT_VERSION >= 0x050000
    #include <QtWidgets/QListView>
#else
    #include <QtGui/QListView>
#endif

#include "core/playlist/containers/Channel.h"

class QAction;
class QMenu;

class Channel;
class PlaylistFilterModel;
class PlaylistModel;

class PlaylistDisplayWidget : public QListView
{
Q_OBJECT
public:
    PlaylistDisplayWidget(QWidget *parent = 0);
    ~PlaylistDisplayWidget();

    Channel *currentChannel() { return _current; }
    QList<Channel *> visibleChannels();

    void playMode();
    void setModel(PlaylistModel *model);

signals:
    void itemSelected(Channel *);
    void scheduleRequested(Channel *);

public slots:
    void channelSelected(Channel *channel);
    void channelSelected(const int &channel);
    void channelSelected(const QString &xmltvId);
    void setFilters(const QString &search);
    void setFilters(const QString &search,
                    const QString &category,
                    const QString &language,
                    const QList<Channel::Type> &types);

private slots:
    void channelSelected(const QModelIndex &index);
    void play();
    void schedule();
    void showMenu(const QPoint &pos);
    void updateSelection(Channel *channel);

private:
    Channel *_current;
    PlaylistModel *_model;
    PlaylistFilterModel *_filterModel;

	QAction *_play;
	QAction *_schedule;
	QMenu *_rightMenu;
	QPoint _currentPos;
};

#endif // TANO_PLAYLISTDISPLAYWIDGET_H_
