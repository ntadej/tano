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

#ifndef TANO_INFOBARWIDGET_H_
#define TANO_INFOBARWIDGET_H_

#include "ScrollingWidget.h"

class InfoBarWidget : public ScrollingWidget
{
Q_OBJECT
public:
    InfoBarWidget(QWidget *parent = 0);
    ~InfoBarWidget();

public slots:
    void clear();
    void setChannelEpg(const QString &now,
                       const QString &next);
    void setChannelInfo(const QString &name,
                        const QString &language = 0);
    void setRecordingInfo(const QString &name,
                          const QString &info = 0);

private:
    QString _spacer;

    QString _info;
    QString _epg;
};

#endif // TANO_INFOBARWIDGET_H_
