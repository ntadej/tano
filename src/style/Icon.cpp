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
#include "style/Icon.h"
#include "style/StyleHelper.h"
#include "style/Theme.h"

#include <QApplication>
#include <QIcon>
#include <QImage>
#include <QMetaEnum>
#include <QPainter>
#include <QPaintEngine>
#include <QWidget>

static QPixmap maskToColorAndAlpha(const QPixmap &mask, const QColor &color)
{
    QImage result(mask.toImage().convertToFormat(QImage::Format_ARGB32));
    result.setDevicePixelRatio(mask.devicePixelRatio());
    QRgb *bitsStart = reinterpret_cast<QRgb*>(result.bits());
    const QRgb *bitsEnd = bitsStart + result.width() * result.height();
    const QRgb tint = color.rgb() & 0x00ffffff;
    const QRgb alpha = QRgb(color.alpha());
    for (QRgb *pixel = bitsStart; pixel < bitsEnd; ++pixel) {
        QRgb pixelAlpha = (((~*pixel) & 0xff) * alpha) >> 8;
        *pixel = (pixelAlpha << 24) | tint;
    }
    return QPixmap::fromImage(result);
}

typedef QPair<QPixmap, QColor> MaskAndColor;
typedef QList<MaskAndColor> MasksAndColors;
static MasksAndColors masksAndColors(const Icon &icon, int dpr)
{
    MasksAndColors result;
    for (const IconMaskAndColor &i: icon) {
        const QString &fileName = i.first;
        const QColor color = Tano::applicationTheme()->color(i.second);
        const QString dprFileName = StyleHelper::availableImageResolutions(i.first).contains(dpr) ?
                    StyleHelper::imageFileWithResolution(fileName, dpr) : fileName;
        result.append(qMakePair(QPixmap(dprFileName), color));
    }
    return result;
}

static void smearPixmap(QPainter *painter, const QPixmap &pixmap, qreal radius)
{
    const qreal nagative = -radius - 0.01; // Workaround for QPainter rounding behavior
    const qreal positive = radius;
    painter->drawPixmap(QPointF(nagative, nagative), pixmap);
    painter->drawPixmap(QPointF(0, nagative), pixmap);
    painter->drawPixmap(QPointF(positive, nagative), pixmap);
    painter->drawPixmap(QPointF(positive, 0), pixmap);
    painter->drawPixmap(QPointF(positive, positive), pixmap);
    painter->drawPixmap(QPointF(0, positive), pixmap);
    painter->drawPixmap(QPointF(nagative, positive), pixmap);
    painter->drawPixmap(QPointF(nagative, 0), pixmap);
}

static QPixmap combinedMask(const MasksAndColors &masks, Icon::IconStyleOptions style)
{
    if (masks.count() == 1)
        return masks.first().first;

    QPixmap result(masks.first().first);
    QPainter p(&result);
    p.setCompositionMode(QPainter::CompositionMode_Darken);
    auto maskImage = masks.constBegin();
    maskImage++;
    for (;maskImage != masks.constEnd(); ++maskImage) {
        if (style & Icon::PunchEdges) {
            p.save();
            p.setOpacity(0.4);
            p.setCompositionMode(QPainter::CompositionMode_Lighten);
            smearPixmap(&p, maskToColorAndAlpha((*maskImage).first, Qt::white), 0.5);
            p.restore();
        }
        p.drawPixmap(0, 0, (*maskImage).first);
    }
    p.end();
    return result;
}

static QPixmap masksToIcon(const MasksAndColors &masks, const QPixmap &combinedMask, Icon::IconStyleOptions style)
{
    QPixmap result(combinedMask.size());
    result.setDevicePixelRatio(combinedMask.devicePixelRatio());
    result.fill(Qt::transparent);
    QPainter p(&result);

    for (MasksAndColors::const_iterator maskImage = masks.constBegin();
         maskImage != masks.constEnd(); ++maskImage) {
        if (style & Icon::PunchEdges && maskImage != masks.constBegin()) {
            // Punch a transparent outline around an overlay.
            p.save();
            p.setOpacity(0.4);
            p.setCompositionMode(QPainter::CompositionMode_DestinationOut);
            smearPixmap(&p, maskToColorAndAlpha((*maskImage).first, Qt::white), 0.5);
            p.restore();
        }
        p.drawPixmap(0, 0, maskToColorAndAlpha((*maskImage).first, (*maskImage).second));
    }

    if (style & Icon::DropShadow) {
        const QPixmap shadowMask = maskToColorAndAlpha(combinedMask, Qt::black);
        p.setCompositionMode(QPainter::CompositionMode_DestinationOver);
        p.setOpacity(0.05);
        p.drawPixmap(QPointF(0, -0.501), shadowMask);
        p.drawPixmap(QPointF(-0.501, 0), shadowMask);
        p.drawPixmap(QPointF(0.5, 0), shadowMask);
        p.drawPixmap(QPointF(0.5, 0.5), shadowMask);
        p.drawPixmap(QPointF(-0.501, 0.5), shadowMask);
        p.setOpacity(0.2);
        p.drawPixmap(0, 1, shadowMask);
    }

    p.end();

    return result;
}

static QPixmap combinedPlainPixmaps(const QVector<IconMaskAndColor> &images)
{
    QPixmap result(StyleHelper::dpiSpecificImageFile(images.first().first));
    auto pixmap = images.constBegin();
    pixmap++;
    for (;pixmap != images.constEnd(); ++pixmap) {
        const QPixmap overlay(StyleHelper::dpiSpecificImageFile((*pixmap).first));
        result.paintEngine()->painter()->drawPixmap(0, 0, overlay);
    }
    return result;
}

Icon::Icon()
{
}

Icon::Icon(std::initializer_list<IconMaskAndColor> args, Icon::IconStyleOptions style)
    : QVector<IconMaskAndColor>(args)
    , m_style(style)
{
}

Icon::Icon(const QString &imageFileName)
    : m_style(None)
{
    append({imageFileName, Theme::Color(-1)});
}

QIcon Icon::icon() const
{
    if (isEmpty()) {
        return QIcon();
    } else if (m_style == None) {
        return QIcon(combinedPlainPixmaps(*this));
    } else {
        QIcon result;
        const MasksAndColors masks = masksAndColors(*this, qRound(qApp->devicePixelRatio()));
        const QPixmap combined = combinedMask(masks, m_style);
        result.addPixmap(masksToIcon(masks, combined, m_style));

        const QColor disabledColor = Tano::applicationTheme()->color(Theme::IconsDisabledColor);
        result.addPixmap(maskToColorAndAlpha(combined, disabledColor), QIcon::Disabled);
        return result;
    }
}

QPixmap Icon::pixmap() const
{
    if (isEmpty()) {
        return QPixmap();
    } else if (m_style == None) {
        return combinedPlainPixmaps(*this);
    } else {
        const MasksAndColors masks =
                masksAndColors(*this, qRound(qApp->devicePixelRatio()));
        const QPixmap combined = combinedMask(masks, m_style);
        return masksToIcon(masks, combined, m_style);
    }
}

QString Icon::imageFileName() const
{
    QTC_ASSERT(length() == 1, return QString());
    return first().first;
}

QIcon Icon::sideBarIcon(const Icon &classic, const Icon &flat)
{
    QIcon result;
    if (Tano::applicationTheme()->flag(Theme::FlatSideBarIcons)) {
        result = flat.icon();
    } else {
        const QPixmap pixmap = classic.pixmap();
        result.addPixmap(pixmap);
        // Ensure that the icon contains a disabled state of that size, since
        // Since we have icons with mixed sizes (e.g. DEBUG_START), and want to
        // avoid that QIcon creates scaled versions of missing QIcon::Disabled
        // sizes.
        result.addPixmap(StyleHelper::disabledSideBarIcon(pixmap), QIcon::Disabled);
    }
    return result;
}

QIcon Icon::modeIcon(const Icon &classic, const Icon &flat, const Icon &flatActive)
{
    QIcon result = sideBarIcon(classic, flat);
    if (Tano::applicationTheme()->flag(Theme::FlatSideBarIcons))
        result.addPixmap(flatActive.pixmap(), QIcon::Active);
    return result;
}

QIcon Icon::combinedIcon(const QList<QIcon> &icons)
{
    QIcon result;
    QWindow *window = QApplication::allWidgets().first()->windowHandle();
    for (const QIcon &icon: icons)
        for (const QIcon::Mode mode: {QIcon::Disabled, QIcon::Normal})
            for (const QSize &size: icon.availableSizes(mode))
                result.addPixmap(icon.pixmap(window, size, mode), mode);
    return result;
}
