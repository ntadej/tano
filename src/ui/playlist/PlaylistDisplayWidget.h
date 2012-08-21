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

#ifndef TANO_PLAYLISTDISPLAYWIDGET_H_
#define TANO_PLAYLISTDISPLAYWIDGET_H_

#include <QtCore/QModelIndex>

#if defined(Qt5)
    #include <QtWidgets/QWidget>
#elif defined(Qt4)
    #include <QtGui/QWidget>
#endif

#include "container/core/Channel.h"

class QAction;
class QMenu;

class Channel;
class PlaylistFilterModel;
class PlaylistFilterWidget;
class PlaylistModel;

namespace Ui
{
    class PlaylistDisplayWidget;
}

class PlaylistDisplayWidget : public QWidget
{
Q_OBJECT
public:
    PlaylistDisplayWidget(QWidget *parent = 0);
    ~PlaylistDisplayWidget();

    Channel *currentChannel() { return _current; }
    PlaylistFilterWidget *filter();
    void filterReset();
    QList<Channel *> visibleChannels();

    void editMode();
    void playMode();
    void refreshModel();
    void setModel(PlaylistModel *model);

protected:
    void changeEvent(QEvent *e);

signals:
    void itemSelected(Channel *);
    void scheduleRequested(Channel *);

public slots:
    void channelSelected(Channel *channel);
    void channelSelected(const int &channel);
    void channelSelected(const QString &xmltvId);

private slots:
    void channelSelected(const QModelIndex &index);
    void play();
    void processFilters(const QString &search,
                        const QString &category,
                        const QString &language,
                        const QList<Channel::Type> &types);
    void schedule();
    void showMenu(const QPoint &pos);
    void updateSelection(Channel *channel);

private:
    Ui::PlaylistDisplayWidget *ui;

    Channel *_current;
    PlaylistModel *_model;
    PlaylistFilterModel *_filterModel;

	QAction *_play;
	QAction *_schedule;
	QMenu *_rightMenu;
	QPoint _currentPos;
};

#endif // TANO_PLAYLISTDISPLAYWIDGET_H_
