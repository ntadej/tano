/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
*
* Copyright (C) 2016 The Qt Company Ltd.
* Contact: https://www.qt.io/licensing/
*
* This file is based on file from Qt Creator.
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

#include "common/qtcassert.h"

#include "Theme.h"
#include "Theme_p.h"

#include <QApplication>
#include <QFileInfo>
#include <QMetaEnum>
#include <QPalette>
#include <QSettings>

ThemePrivate::ThemePrivate()
    : widgetStyle(Theme::StyleDefault)
{
    const QMetaObject &m = Theme::staticMetaObject;
    colors.resize        (m.enumerator(m.indexOfEnumerator("Color")).keyCount());
    imageFiles.resize    (m.enumerator(m.indexOfEnumerator("ImageFile")).keyCount());
    gradients.resize     (m.enumerator(m.indexOfEnumerator("Gradient")).keyCount());
    flags.resize         (m.enumerator(m.indexOfEnumerator("Flag")).keyCount());
}

namespace Tano {

static Theme *m_applicationTheme = 0;

Theme *applicationTheme()
{
    return m_applicationTheme;
}

void setApplicationTheme(Theme *theme)
{
    if (m_applicationTheme == theme)
        return;
    delete m_applicationTheme;
    m_applicationTheme = theme;
}

} // namespace Tano

Theme::Theme(const QString &id, QObject *parent)
  : QObject(parent)
  , d(new ThemePrivate)
{
    d->id = id;
}

Theme::~Theme()
{
    delete d;
}

Theme::WidgetStyle Theme::widgetStyle() const
{
    return d->widgetStyle;
}

QStringList Theme::preferredStyles() const
{
    return d->preferredStyles;
}

QString Theme::defaultTextEditorColorScheme() const
{
    return d->defaultTextEditorColorScheme;
}

QString Theme::id() const
{
    return d->id;
}

bool Theme::flag(Theme::Flag f) const
{
    return d->flags[f];
}

QColor Theme::color(Theme::Color role) const
{
    return d->colors[role].first;
}

QString Theme::imageFile(Theme::ImageFile imageFile, const QString &fallBack) const
{
    const QString &file = d->imageFiles.at(imageFile);
    return file.isEmpty() ? fallBack : file;
}

QGradientStops Theme::gradient(Theme::Gradient role) const
{
    return d->gradients[role];
}

QPair<QColor, QString> Theme::readNamedColor(const QString &color) const
{
    if (d->palette.contains(color))
        return qMakePair(d->palette[color], color);
    if (color == QLatin1String("style"))
        return qMakePair(QColor(), QString());

    bool ok = true;
    const QRgb rgba = color.toLongLong(&ok, 16);
    if (!ok) {
        qWarning("Color \"%s\" is neither a named color nor a valid color", qPrintable(color));
        return qMakePair(Qt::black, QString());
    }
    return qMakePair(QColor::fromRgba(rgba), QString());
}

QString Theme::filePath() const
{
    return d->fileName;
}

QString Theme::displayName() const
{
    return d->displayName;
}

void Theme::setDisplayName(const QString &name)
{
    d->displayName = name;
}

QVariantHash Theme::values() const
{
    QVariantHash result;
    const QMetaObject &m = *metaObject();
    {
        const QMetaEnum e = m.enumerator(m.indexOfEnumerator("Color"));
        for (int i = 0, total = e.keyCount(); i < total; ++i) {
            const QString key = QLatin1String(e.key(i));
            const QPair<QColor, QString> &var = d->colors.at(i);
            result.insert(key, var.first);
        }
    }
    {
        const QMetaEnum e = m.enumerator(m.indexOfEnumerator("Flag"));
        for (int i = 0, total = e.keyCount(); i < total; ++i) {
            const QString key = QLatin1String(e.key(i));
            result.insert(key, flag(static_cast<Theme::Flag>(i)));
        }
    }
    {
        const QMetaEnum e = m.enumerator(m.indexOfEnumerator("WidgetStyle"));
        result.insert(QLatin1String("WidgetStyle"), QLatin1String(e.valueToKey(widgetStyle())));
    }
    return result;
}

static QColor readColor(const QString &color)
{
    bool ok = true;
    const QRgb rgba = color.toLongLong(&ok, 16);
    return QColor::fromRgba(rgba);
}

void Theme::readSettings(QSettings &settings)
{
    d->fileName = settings.fileName();
    const QMetaObject &m = *metaObject();

    {
        d->displayName = settings.value(QLatin1String("ThemeName"), QLatin1String("unnamed")).toString();
        d->preferredStyles = settings.value(QLatin1String("PreferredStyles")).toStringList();
        d->preferredStyles.removeAll(QLatin1String(""));
        d->defaultTextEditorColorScheme =
                settings.value(QLatin1String("DefaultTextEditorColorScheme")).toString();
    }
    {
        settings.beginGroup(QLatin1String("Palette"));
        foreach (const QString &key, settings.allKeys()) {
            QColor c = readColor(settings.value(key).toString());
            d->palette[key] = c;
        }
        settings.endGroup();
    }
    {
        settings.beginGroup(QLatin1String("Style"));
        QMetaEnum e = m.enumerator(m.indexOfEnumerator("WidgetStyle"));
        QString val = settings.value(QLatin1String("WidgetStyle")).toString();
        d->widgetStyle = static_cast<Theme::WidgetStyle>(e.keysToValue (val.toLatin1().data()));
        settings.endGroup();
    }
    {
        settings.beginGroup(QLatin1String("Colors"));
        QMetaEnum e = m.enumerator(m.indexOfEnumerator("Color"));
        for (int i = 0, total = e.keyCount(); i < total; ++i) {
            const QString key = QLatin1String(e.key(i));
            if (!settings.contains(key)) {
                qWarning("Theme \"%s\" misses color setting for key \"%s\".",
                         qPrintable(d->fileName), qPrintable(key));
                continue;
            }
            d->colors[i] = readNamedColor(settings.value(key).toString());
        }
        settings.endGroup();
    }
    {
        settings.beginGroup(QLatin1String("ImageFiles"));
        QMetaEnum e = m.enumerator(m.indexOfEnumerator("ImageFile"));
        for (int i = 0, total = e.keyCount(); i < total; ++i) {
            const QString key = QLatin1String(e.key(i));
            d->imageFiles[i] = settings.value(key).toString();
        }
        settings.endGroup();
    }
    {
        settings.beginGroup(QLatin1String("Gradients"));
        QMetaEnum e = m.enumerator(m.indexOfEnumerator("Gradient"));
        for (int i = 0, total = e.keyCount(); i < total; ++i) {
            const QString key = QLatin1String(e.key(i));
            QGradientStops stops;
            int size = settings.beginReadArray(key);
            for (int j = 0; j < size; ++j) {
                settings.setArrayIndex(j);
                QTC_ASSERT(settings.contains(QLatin1String("pos")), return);;
                double pos = settings.value(QLatin1String("pos")).toDouble();
                QTC_ASSERT(settings.contains(QLatin1String("color")), return);;
                QColor c = readColor(settings.value(QLatin1String("color")).toString());
                stops.append(qMakePair(pos, c));
            }
            settings.endArray();
            d->gradients[i] = stops;
        }
        settings.endGroup();
    }
    {
        settings.beginGroup(QLatin1String("Flags"));
        QMetaEnum e = m.enumerator(m.indexOfEnumerator("Flag"));
        for (int i = 0, total = e.keyCount(); i < total; ++i) {
            const QString key = QLatin1String(e.key(i));
            QTC_ASSERT(settings.contains(key), return);;
            d->flags[i] = settings.value(key).toBool();
        }
        settings.endGroup();
    }
}

QPalette Theme::initialPalette()
{
    static QPalette palette = QApplication::palette();
    return palette;
}

QPalette Theme::palette() const
{
    QPalette pal = initialPalette();
    if (!flag(DerivePaletteFromTheme))
        return pal;

    // FIXME: introduce some more color roles for this

    pal.setColor(QPalette::Window,          color(Theme::BackgroundColorNormal));
    pal.setBrush(QPalette::WindowText,      color(Theme::TextColorNormal));
    pal.setColor(QPalette::Base,            color(Theme::BackgroundColorNormal));
    pal.setColor(QPalette::AlternateBase,   color(Theme::BackgroundColorAlternate));
    pal.setColor(QPalette::Button,          color(Theme::BackgroundColorDark));
    pal.setColor(QPalette::BrightText,      Qt::red);
    pal.setBrush(QPalette::Text,            color(Theme::TextColorNormal));
    pal.setBrush(QPalette::ButtonText,      color(Theme::TextColorNormal));
    pal.setBrush(QPalette::ToolTipBase,     color(Theme::BackgroundColorSelected));
    pal.setColor(QPalette::Highlight,       color(Theme::BackgroundColorSelected));
    pal.setColor(QPalette::Dark,            color(Theme::BackgroundColorDark));
    pal.setColor(QPalette::HighlightedText, Qt::white);
    pal.setColor(QPalette::ToolTipText,     color(Theme::TextColorNormal));
    pal.setColor(QPalette::Link,            color(Theme::TextColorLink));
    pal.setColor(QPalette::LinkVisited,     color(Theme::TextColorLinkVisited));
    pal.setColor(QPalette::Disabled, QPalette::Window,     color(Theme::BackgroundColorDisabled));
    pal.setBrush(QPalette::Disabled, QPalette::WindowText, color(Theme::TextColorDisabled));
    pal.setColor(QPalette::Disabled, QPalette::Base,       color(Theme::BackgroundColorDisabled));
    pal.setBrush(QPalette::Disabled, QPalette::Text,       color(Theme::TextColorDisabled));
    return pal;
}
