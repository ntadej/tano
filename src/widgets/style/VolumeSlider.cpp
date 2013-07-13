/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
*
* This file is part of Amarok.
* Copyright (C) 2003 by Mark Kretschmann <markey@web.de>
* Copyright (C) 2005 by Gábor Lehel <illissius@gmail.com>
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

#include "VolumeSlider.h"

#include <QApplication>
#include <QBitmap>
#include <QBrush>
#include <QImage>
#include <QPainter>
#include <QSize>
#include <QTimer>
#include <QStyle>
#include <QMenu>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QStyleOptionSlider>


VolumeSliderBase::VolumeSliderBase(QWidget *parent,
                                   uint max)
    : VlcWidgetVolumeSlider(parent),
      _sliding(false),
      _outside(false),
      _prevValue(0)
{
    setRange(0, max);
}

void VolumeSliderBase::wheelEvent(QWheelEvent *e)
{
    if( orientation() == Qt::Vertical ) {
        // Will be handled by the parent widget
        e->ignore();
        return;
    }

    // Position Slider (horizontal)
    int step = e->delta() * 1500 / 18;
    int nval = QSlider::value() + step;
    nval = qMax(nval, minimum());
    nval = qMin(nval, maximum());

    QSlider::setValue(nval);

    emit sliderReleased(value());
}

void VolumeSliderBase::mouseMoveEvent(QMouseEvent *e)
{
    if (_sliding) {
        //feels better, but using set value of 20 is bad of course
        QRect rect(-20, -20, width()+40, height()+40);

        if (orientation() == Qt::Horizontal && !rect.contains(e->pos())) {
            if (!_outside)
                QSlider::setValue(_prevValue);
            _outside = true;
        } else {
            _outside = false;
            slideEvent(e);
            emit sliderMoved(value());
        }
    }
    else QSlider::mouseMoveEvent(e);
}

void VolumeSliderBase::slideEvent(QMouseEvent *e)
{
    QStyleOptionSlider option;
    initStyleOption(&option);
    QRect sliderRect(style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, this));

    QSlider::setValue( orientation() == Qt::Horizontal
        ? ((QApplication::layoutDirection() == Qt::RightToLeft) ?
           QStyle::sliderValueFromPosition(minimum(), maximum(), width() - (e->pos().x() - sliderRect.width()/2),  width()  + sliderRect.width(), true )
             : QStyle::sliderValueFromPosition(minimum(), maximum(), e->pos().x() - sliderRect.width()/2,  width()  - sliderRect.width() ) )
        : QStyle::sliderValueFromPosition(minimum(), maximum(), e->pos().y() - sliderRect.height()/2, height() - sliderRect.height() ) );
}

void VolumeSliderBase::mousePressEvent(QMouseEvent *e)
{
    VlcWidgetVolumeSlider::mousePressEvent(e);

    QStyleOptionSlider option;
    initStyleOption(&option);
    QRect sliderRect(style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, this));

    _sliding   = true;
    _prevValue = QSlider::value();

    if (!sliderRect.contains(e->pos()))
        mouseMoveEvent(e);
}

void VolumeSliderBase::mouseReleaseEvent(QMouseEvent *e)
{
    VlcWidgetVolumeSlider::mouseReleaseEvent(e);

    if(!_outside && QSlider::value() != _prevValue)
       emit sliderReleased(value());

    _sliding = false;
    _outside = false;
}

void VolumeSliderBase::setValue(int newValue)
{
    //don't adjust the slider while the user is dragging it!

    if (!_sliding || _outside)
        QSlider::setValue(adjustValue(newValue));
    else
        _prevValue = newValue;
}


VolumeSlider::VolumeSlider(QWidget *parent,
                           uint max)
    : VolumeSliderBase(parent, max),
      _animCount(0),
      _animTimer(new QTimer(this)),
      _pixmapInset(QPixmap(":/style/volumeslider-inset.png"))
{
    setFocusPolicy(Qt::NoFocus);

    // BEGIN Calculate handle animation pixmaps for mouse-over effect
    QImage pixmapHandle    (":/style/volumeslider-handle.png");
    QImage pixmapHandleGlow(":/style/volumeslider-handle_glow.png");

    float opacity = 0.0;
    const float step = 1.0 / ANIM_MAX;
    QImage dst;
    for (int i = 0; i < ANIM_MAX; ++i) {
        dst = pixmapHandle.copy();

        QPainter p(&dst);
        p.setOpacity(opacity);
        p.drawImage(0, 0, pixmapHandleGlow);
        p.end();

        _handlePixmaps.append(QPixmap::fromImage(dst));
        opacity += step;
    }
    // END

    generateGradient();

    setMinimumWidth(_pixmapInset.width());
    setMinimumHeight(_pixmapInset.height());

    setMaximumWidth(_pixmapInset.width());
    setMaximumHeight(_pixmapInset.height());

    connect(_animTimer, SIGNAL(timeout()), this, SLOT(slotAnimTimer()));
}

void VolumeSlider::generateGradient()
{
    const QImage mask(":/style/volumeslider-gradient.png");

    QImage gradient_image(mask.size(), QImage::Format_ARGB32_Premultiplied);
    QPainter p(&gradient_image);

    QLinearGradient gradient(gradient_image.rect().topLeft(),
                             gradient_image.rect().topRight());
    gradient.setColorAt(0, palette().color(QPalette::Background));
    gradient.setColorAt(1, palette().color(QPalette::Highlight));
    p.fillRect(gradient_image.rect(), QBrush(gradient));

    p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p.drawImage(0, 0, mask);
    p.end();

    _pixmapGradient = QPixmap::fromImage(gradient_image);
}

void VolumeSlider::slotAnimTimer()
{
    if (_animEnter) {
        _animCount++;
        update();
        if (_animCount == ANIM_MAX - 1)
            _animTimer->stop();
    } else {
        _animCount--;
        update();
        if (_animCount == 0)
            _animTimer->stop();
    }
}

void VolumeSlider::mousePressEvent(QMouseEvent *e)
{
    if(e->button() != Qt::RightButton) {
        VolumeSliderBase::mousePressEvent(e);
        slideEvent(e);
    }
}
                
void VolumeSlider::contextMenuEvent(QContextMenuEvent *e)
{
    QMap<QAction*, int> values;
    QMenu menu;
    menu.setTitle("Volume");
    values[menu.addAction("100%")] = 100;
    values[menu.addAction("80%")] = 80;
    values[menu.addAction("60%")] = 60;
    values[menu.addAction("40%")] = 40;
    values[menu.addAction("20%")] = 20;
    values[menu.addAction("0%")] = 0;

    QAction* ret = menu.exec(mapToGlobal(e->pos()));
    if(ret)
    {
        QSlider::setValue(values[ret]);
        emit sliderReleased(values[ret]);
    }
}

void VolumeSlider::slideEvent(QMouseEvent *e)
{
    QSlider::setValue(QStyle::sliderValueFromPosition(minimum(), maximum(), e->pos().x(), width()-2));
}

void VolumeSlider::wheelEvent(QWheelEvent *e)
{
    const uint step = e->delta() / 30;
    QSlider::setValue(QSlider::value() + step);

    emit sliderReleased(value());
}

void VolumeSlider::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QPainter p(this);

    const int padding = 7;
    const int offset = int(double((width() - 2 * padding) * value()) / maximum());

    p.drawPixmap(0, 0, _pixmapGradient, 0, 0, offset + padding, 0);
    p.drawPixmap(0, 0, _pixmapInset);
    p.drawPixmap(offset - _handlePixmaps[0].width() / 2 + padding, 0, _handlePixmaps[_animCount]);

    // Draw percentage number
    p.setPen(palette().color(QPalette::Disabled, QPalette::Text ).dark());
    QFont font;
    font.setPixelSize(9);
    p.setFont(font);
    const QRect rect(0, 0, 34, 15);
    p.drawText(rect, Qt::AlignRight | Qt::AlignVCenter, QString::number(value()) + '%');
}

void VolumeSlider::enterEvent(QEvent *e)
{
    Q_UNUSED(e)

    _animEnter = true;
    _animCount = 0;

    _animTimer->start( ANIM_INTERVAL );
}

void VolumeSlider::leaveEvent(QEvent *e)
{
    Q_UNUSED(e)
    // This can happen if you enter and leave the widget quickly
    if (_animCount == 0)
        _animCount = 1;

    _animEnter = false;
    _animTimer->start(ANIM_INTERVAL);
}

void VolumeSlider::paletteChange(const QPalette& palette)
{
    Q_UNUSED(palette)

    generateGradient();
}
