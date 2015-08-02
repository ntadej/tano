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

#ifndef TANO_VOLUMESLIDER_H_
#define TANO_VOLUMESLIDER_H_

#include <QPixmap>
#include <QSlider>

#include <VLCQtWidgets/WidgetVolumeSlider.h>

class QPalette;
class QTimer;

class VolumeSliderBase : public VlcWidgetVolumeSlider
{
Q_OBJECT
public:
    VolumeSliderBase(QWidget *parent,
                     uint max = 100);

    virtual void setValue(int);
    int value() const { return adjustValue(QSlider::value()); }

signals:
    //we emit this when the user has specifically changed the slider
    //so connect to it if valueChanged() is too generic
    //Qt also emits valueChanged(int)
    void sliderReleased(int);

protected:
    virtual void wheelEvent(QWheelEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void slideEvent(QMouseEvent *e);

    bool _sliding;

    /// we flip the value for vertical sliders
    int adjustValue(int v) const
    {
       int mp = (minimum() + maximum()) / 2;
       return orientation() == Qt::Vertical ? mp - (v - mp) : v;
    }

private:
    bool _outside;
    int  _prevValue;
};

class VolumeSlider: public VolumeSliderBase
{
Q_OBJECT
public:
    VolumeSlider(QWidget *parent, uint max = 100);

protected:
    virtual void paintEvent(QPaintEvent *e);
    virtual void enterEvent(QEvent *e);
    virtual void leaveEvent(QEvent *e);
    virtual void paletteChange(const QPalette &palette);
    virtual void slideEvent(QMouseEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void contextMenuEvent(QContextMenuEvent *e);
    virtual void wheelEvent(QWheelEvent *e);

private slots:
    virtual void slotAnimTimer();

private:
    void generateGradient();

    static const int ANIM_INTERVAL = 18;
    static const int ANIM_MAX = 18;

    bool    _animEnter;
    int     _animCount;
    QTimer *_animTimer;

    QPixmap _pixmapInset;
    QPixmap _pixmapGradient;

    QList<QPixmap> _handlePixmaps;
};

#endif
