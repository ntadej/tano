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

#ifndef TANO_PLAYLISTFILTERWIDGET_H_
#define TANO_PLAYLISTFILTERWIDGET_H_

#include <QtCore/QModelIndex>

#if defined(Qt5)
    #include <QtWidgets/QWidget>
#elif defined(Qt4)
    #include <QtGui/QWidget>
#endif

#include "container/core/Channel.h"

namespace Ui
{
    class PlaylistFilterWidget;
}

class PlaylistFilterWidget : public QWidget
{
Q_OBJECT
public:
    PlaylistFilterWidget(QWidget *parent = 0);
    ~PlaylistFilterWidget();

    void editMode();
    void refreshModel(const QStringList &categories,
                      const QStringList &languages);

protected:
    void changeEvent(QEvent *e);

signals:
    void filters(const QString &,
                 const QString &,
                 const QString &,
                 const QList<Channel::Type> &);

private slots:
    void processFilters();

private:
    Ui::PlaylistFilterWidget *ui;
};

#endif // TANO_PLAYLISTFILTERWIDGET_H_
