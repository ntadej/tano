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

#include "StyleAnimator.h"

#include <QStyleOption>

Animation *StyleAnimator::widgetAnimation(const QWidget *widget) const
{
    if (!widget)
        return 0;
    foreach (Animation *a, _animations) {
        if (a->widget() == widget)
            return a;
    }
    return 0;
}

void Animation::paint(QPainter *painter,
                      const QStyleOption *option)
{
    Q_UNUSED(option)
    Q_UNUSED(painter)
}

void Animation::drawBlendedImage(QPainter *painter,
                                 QRect rect,
                                 float alpha)
{
    if (_secondaryImage.isNull() || _primaryImage.isNull())
        return;

    if (_tempImage.isNull())
        _tempImage = _secondaryImage;

    const int a = qRound(alpha*256);
    const int ia = 256 - a;
    const int sw = _primaryImage.width();
    const int sh = _primaryImage.height();
    const int bpl = _primaryImage.bytesPerLine();
    switch (_primaryImage.depth()) {
    case 32:
        {
            uchar *mixed_data = _tempImage.bits();
            const uchar *back_data = _primaryImage.bits();
            const uchar *front_data = _secondaryImage.bits();
            for (int sy = 0; sy < sh; sy++) {
                quint32 *mixed = (quint32*)mixed_data;
                const quint32* back = (const quint32*)back_data;
                const quint32* front = (const quint32*)front_data;
                for (int sx = 0; sx < sw; sx++) {
                    quint32 bp = back[sx];
                    quint32 fp = front[sx];
                    mixed[sx] =  qRgba ((qRed(bp)*ia + qRed(fp)*a)>>8,
                                        (qGreen(bp)*ia + qGreen(fp)*a)>>8,
                                        (qBlue(bp)*ia + qBlue(fp)*a)>>8,
                                        (qAlpha(bp)*ia + qAlpha(fp)*a)>>8);
                }
                mixed_data += bpl;
                back_data += bpl;
                front_data += bpl;
            }
        }
    default:
        break;
    }
    painter->drawImage(rect, _tempImage);
}

void Transition::paint(QPainter *painter,
                       const QStyleOption *option)
{
    float alpha = 1.0;
    if (_duration > 0) {
        QTime current = QTime::currentTime();

        if (_startTime > current)
            _startTime = current;

        int timeDiff = _startTime.msecsTo(current);
        alpha = timeDiff/(float)_duration;
        if (timeDiff > _duration) {
            _running = false;
            alpha = 1.0;
        }
    }
    else {
        _running = false;
    }
    drawBlendedImage(painter, option->rect, alpha);
}

void StyleAnimator::timerEvent(QTimerEvent *)
{
    for (int i = _animations.size() - 1 ; i >= 0 ; --i) {
        if (_animations[i]->widget())
            _animations[i]->widget()->update();

        if (!_animations[i]->widget() ||
            !_animations[i]->widget()->isEnabled() ||
            !_animations[i]->widget()->isVisible() ||
            _animations[i]->widget()->window()->isMinimized() ||
            !_animations[i]->running())
        {
            Animation *a = _animations.takeAt(i);
            delete a;
        }
    }
    if (_animations.size() == 0 && _animationTimer.isActive())
        _animationTimer.stop();
}

void StyleAnimator::stopAnimation(const QWidget *w)
{
    for (int i = _animations.size() - 1 ; i >= 0 ; --i) {
        if (_animations[i]->widget() == w) {
            Animation *a = _animations.takeAt(i);
            delete a;
            break;
        }
    }
}

void StyleAnimator::startAnimation(Animation *t)
{
    stopAnimation(t->widget());
    _animations.append(t);
    if (_animations.size() > 0 && !_animationTimer.isActive())
        _animationTimer.start(35, this);
}
