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

#include <QPainter>
#include <QPropertyAnimation>

#include "IconButton.h"

#define FADE_TIME 160

IconButton::IconButton(QWidget *parent)
    : QAbstractButton(parent),
      _autoHide(false)
{
    setCursor(Qt::ArrowCursor);
    setFocusPolicy(Qt::NoFocus);
}

void IconButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    qreal ratio = _pixmap.devicePixelRatio();

    QRect pixmapRect = QRect(0, 0, _pixmap.width() / ratio, _pixmap.height() / ratio);
    pixmapRect.moveCenter(rect().center());

    if (_autoHide)
        painter.setOpacity(_iconOpacity);

    painter.drawPixmap(pixmapRect, _pixmap);
}

void IconButton::animateShow(bool visible)
{
    if (visible) {
        QPropertyAnimation *animation = new QPropertyAnimation(this, "iconOpacity");
        animation->setDuration(FADE_TIME);
        animation->setEndValue(1.0);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    } else {
        QPropertyAnimation *animation = new QPropertyAnimation(this, "iconOpacity");
        animation->setDuration(FADE_TIME);
        animation->setEndValue(0.0);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

