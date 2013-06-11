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

#ifndef TANO_EPGSCHEDULEFULL_H_
#define TANO_EPGSCHEDULEFULL_H_

#if defined(Qt5)
    #include <QtWidgets/QWidget>
#elif defined(Qt4)
    #include <QtGui/QWidget>
#endif

#include "core/Common.h"

namespace Ui
{
    class EpgScheduleFull;
}

class QMenu;
class QWidgetAction;

class Channel;
class EpgScheduleChannel;
class OsdFloat;
class PlaylistModel;
class XmltvProgramme;

class EpgScheduleFull : public QWidget
{
Q_OBJECT
public:
    EpgScheduleFull(QWidget *parent = 0);
    ~EpgScheduleFull();

    void refreshPlaylistModel();
    void setFullscreen(const bool &enabled,
                       OsdFloat *widget = 0);
    void setPlaylistModel(PlaylistModel *model);
    EpgScheduleChannel *schedule();

protected:
    void changeEvent(QEvent *e);

signals:
    void itemSelected(const QString &);
    void requestEpg(const QString &,
                    const Tano::Id &);
    void requestRecord(const QString &);

public slots:
    void openSchedule(Channel *channel = 0);

private slots:
    void channel(Channel *channel);
    void closeOsd();

private:
    Ui::EpgScheduleFull *ui;

    bool _fullscreen;
    OsdFloat *_osd;
};

#endif // TANO_EPGSCHEDULEFULL_H_
