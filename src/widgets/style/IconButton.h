/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
*
* This file is part of Qt Creator.
* Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
* Contact: http://www.qt-project.org/legal
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

#ifndef TANO_ICONBUTTON_H_
#define TANO_ICONBUTTON_H_

#include <QAbstractButton>

class IconButton: public QAbstractButton
{
Q_OBJECT
Q_PROPERTY(float iconOpacity READ iconOpacity WRITE setIconOpacity)
Q_PROPERTY(bool autoHide READ hasAutoHide WRITE setAutoHide)
Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap)
public:
    explicit IconButton(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void setPixmap(const QPixmap &pixmap) { _pixmap = pixmap; update(); }
    QPixmap pixmap() const { return _pixmap; }
    float iconOpacity() { return _iconOpacity; }
    void setIconOpacity(float value) { _iconOpacity = value; update(); }
    void animateShow(bool visible);

    void setAutoHide(bool hide) { _autoHide = hide; }
    bool hasAutoHide() const { return _autoHide; }
private:
    float _iconOpacity;
    bool _autoHide;
    QPixmap _pixmap;
};

#endif // TANO_ICONBUTTON_H_
