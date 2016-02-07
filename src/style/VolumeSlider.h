/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
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
#include <QList>

#include <VLCQtWidgets/WidgetVolumeSlider.h>

class QPalette;
class QTimer;

class Slider : public VlcWidgetVolumeSlider {
  Q_OBJECT

 public:
  Slider(QWidget*, uint max = 100);

  virtual void setValue(int);

  // WARNING non-virtual - and thus only really intended for internal use
  // this is a major flaw in the class presently, however it suits our
  // current needs fine
  int value() const { return adjustValue(QSlider::value()); }

signals:
  // we emit this when the user has specifically changed the slider
  // so connect to it if valueChanged() is too generic
  // Qt also emits valueChanged( int )
  void sliderReleased(int);

 protected:
  virtual void wheelEvent(QWheelEvent*);
  virtual void mouseMoveEvent(QMouseEvent*);
  virtual void mouseReleaseEvent(QMouseEvent*e);
  virtual void mousePressEvent(QMouseEvent*);
  virtual void slideEvent(QMouseEvent*);

  bool m_sliding;

  /// we flip the value for vertical sliders
  int adjustValue(int v) const {
    int mp = (minimum() + maximum()) / 2;
    return orientation() == Qt::Vertical ? mp - (v - mp) : v;
  }

 private:
  bool m_outside;
  int m_prevValue;

  Slider(const Slider&);             // undefined
  Slider& operator=(const Slider&);  // undefined
};

class VolumeSlider : public Slider {
  Q_OBJECT

 public:
  VolumeSlider(QWidget* parent, uint max = 100);

 protected:
  virtual void paintEvent(QPaintEvent*);
  virtual void enterEvent(QEvent*);
  virtual void leaveEvent(QEvent*);
  virtual void paletteChange(const QPalette&);
  virtual void slideEvent(QMouseEvent*);
  virtual void mousePressEvent(QMouseEvent*);
  virtual void contextMenuEvent(QContextMenuEvent*);
  virtual void wheelEvent(QWheelEvent* e);

 private slots:
  virtual void slotAnimTimer();

 private:
  void generateGradient();
  QPixmap drawVolumePixmap() const;
  void drawVolumeSliderHandle();

  VolumeSlider(const VolumeSlider&);             // undefined
  VolumeSlider& operator=(const VolumeSlider&);  // undefined

  ////////////////////////////////////////////////////////////////
  static const int ANIM_INTERVAL = 18;
  static const int ANIM_MAX = 18;

  bool m_animEnter;
  int m_animCount;
  QTimer* m_animTimer;

  QPixmap m_pixmapInset;
  QPixmap m_pixmapGradient;

  QColor m_previous_theme_text_color;
  QColor m_previous_theme_highlight_color;

  QList<QPixmap> m_handlePixmaps;
};

#endif
