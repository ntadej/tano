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

#ifndef TANO_STYLEHELPER_H_
#define TANO_STYLEHELPER_H_

#include <QStyle>

#include "WidgetsSharedExport.h"

class QPalette;
class QPainter;
class QRect;
// Note, this is exported but in a private header as qtopengl depends on it.
// We should consider adding this as a public helper function.
void qt_blurImage(QPainter *p,
                  QImage &blurImage,
                  qreal radius,
                  bool quality,
                  bool alphaOnly,
                  int transposed = 0);

// Helper class holding all custom color values
class TANO_WIDGETS_EXPORT StyleHelper
{
public:
    static const unsigned int DEFAULT_BASE_COLOR = 0x666666;
    static const int progressFadeAnimationDuration = 600;

    // Height of the project explorer navigation bar
    static int navigationWidgetHeight() { return 24; }
    static qreal sidebarFontSize();
    static QPalette sidebarFontPalette(const QPalette &original);

    // This is our color table, all colors derive from baseColor
    static QColor requestedBaseColor() { return m_requestedBaseColor; }
    static QColor baseColor(bool lightColored = false);
    static QColor panelTextColor(bool lightColored = false);
    static QColor highlightColor(bool lightColored = false);
    static QColor shadowColor(bool lightColored = false);
    static QColor borderColor(bool lightColored = false);
    static QColor buttonTextColor() { return QColor(0x4c4c4c); }
    static QColor mergedColors(const QColor &colorA,
                               const QColor &colorB,
                               int factor = 50);

    static QColor sidebarHighlight() { return QColor(255, 255, 255, 40); }
    static QColor sidebarShadow() { return QColor(0, 0, 0, 40); }

    // Sets the base color and makes sure all top level widgets are updated
    static void setBaseColor(const QColor &color);

    // Draws a shaded anti-aliased arrow
    static void drawArrow(QStyle::PrimitiveElement element,
                          QPainter *painter,
                          const QStyleOption *option,
                          qreal devicePixelRatio);

    // Gradients used for panels
    static void horizontalGradient(QPainter *painter,
                                   const QRect &spanRect,
                                   const QRect &clipRect,
                                   bool lightColored = false);
    static void verticalGradient(QPainter *painter,
                                 const QRect &spanRect,
                                 const QRect &clipRect,
                                 bool lightColored = false);
    static void menuGradient(QPainter *painter,
                             const QRect &spanRect,
                             const QRect &clipRect);
    static bool usePixmapCache() { return true; }

    static void drawIconWithShadow(const QIcon &icon,
                                   const QRect &rect,
                                   QPainter *p,
                                   QIcon::Mode iconMode,
                                   int dipRadius = 3,
                                   const QColor &color = QColor(0, 0, 0, 130),
                                   const QPoint &dipOffset = QPoint(1, -2));
    static void drawCornerImage(const QImage &img,
                                QPainter *painter,
                                QRect rect,
                                int left = 0,
                                int top = 0,
                                int right = 0,
                                int bottom = 0);

    static void tintImage(QImage &img,
                          const QColor &tintColor);
    static QLinearGradient statusBarGradient(const QRect &statusBarRect);

private:
    static QColor m_baseColor;
    static QColor m_requestedBaseColor;
};

#endif // TANO_STYLEHELPER_H_
