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

#ifndef TANO_INFOBARWIDGET_H_
#define TANO_INFOBARWIDGET_H_

#include <QtCore/QTimer>
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>

class InfoBarWidget : public QScrollArea
{
Q_OBJECT
public:
    InfoBarWidget(QWidget *parent = 0);
    ~InfoBarWidget();

public slots:
    void clear();
    void setInfo(const QString &channel,
                 const QString &language);
    void setEpg(const QString &now,
                const QString &next);

signals:
    void open(const QString);

private slots:
    void scroll();

private:
    bool _direction;
    QTimer *_timer;

    QLabel *_labelChannel;
    QLabel *_labelLanguage;
    QLabel *_labelNext;
    QLabel *_labelNow;
};

#endif // TANO_INFOBARWIDGET_H_
