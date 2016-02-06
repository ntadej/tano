/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
*
* Copyright (C) 2016 The Qt Company Ltd.
* Contact: https://www.qt.io/licensing/
*
* This file is based on file from Qt Application.
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

#ifndef TANO_THEME_P_H_
#define TANO_THEME_P_H_

#include "Theme.h"

#include <QColor>
#include <QMap>

class ThemePrivate
{
public:
    ThemePrivate();

    QString id;
    QString fileName;
    QString displayName;
    QStringList preferredStyles;
    QString defaultTextEditorColorScheme;
    QVector<QPair<QColor, QString> > colors;
    QVector<QString> imageFiles;
    QVector<QGradientStops> gradients;
    QVector<bool> flags;
    Theme::WidgetStyle widgetStyle;
    QMap<QString, QColor> palette;
};

namespace Tano {
void setApplicationTheme(Theme *theme);
}

#endif // TANO_THEME_P_H_
