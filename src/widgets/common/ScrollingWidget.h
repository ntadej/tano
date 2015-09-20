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

#ifndef TANO_SCROLLINGWIDGET_H_
#define TANO_SCROLLINGWIDGET_H_

#include <QtCore/QTimer>
#include <QtWidgets/QScrollArea>

class QLabel;
class QResizeEvent;

class ScrollingWidget : public QScrollArea
{
Q_OBJECT
public:
    ScrollingWidget(QWidget *parent = 0);
    ~ScrollingWidget();

    void removeBorder();

protected:
    void resizeEvent(QResizeEvent *event);

public slots:
    void setText(const QString &text = "");

signals:
    void open(const QString);

private slots:
    void scroll();

private:
    bool _direction;
    QTimer *_timer;

    QLabel *_label;
};

#endif // TANO_SCROLLINGWIDGET_H_
