/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
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

#include <QtGui/QColor>
#include <QtGui/QFont>

#include "MobileConstants.h"

QDeclarativePropertyMap *MobileConstants::uiConstants()
{
    QDeclarativePropertyMap *uiConstantsData = new QDeclarativePropertyMap();
    uiConstantsData->insert("DefaultMargin", QVariant(16));
    uiConstantsData->insert("ButtonSpacing", QVariant(6));

    uiConstantsData->insert("HeaderDefaultHeight", QVariant(72));
    uiConstantsData->insert("HeaderDefaultTopSpacing", QVariant(20));

    uiConstantsData->insert("ListItemHeightSmall", QVariant(64));
    uiConstantsData->insert("ListItemHeightDefault", QVariant(80));

    // Colors
    uiConstantsData->insert("ColorId", QVariant("color8"));

    uiConstantsData->insert("FieldLabelColor", QVariant(QColor("#505050")));
    uiConstantsData->insert("HeaderColor", QVariant(QColor("#ffffff")));
    uiConstantsData->insert("HighlightColor", QVariant(QColor("#1c51ae")));

    // Fonts
    QFont bodyTextFont;
    bodyTextFont.setFamily("Nokia Pure Text Light");
    bodyTextFont.setPixelSize(24);
    uiConstantsData->insert("BodyTextFont", QVariant(bodyTextFont));

    QFont headerFont;
    headerFont.setFamily("Nokia Pure Text Light");
    headerFont.setPixelSize(32);
    uiConstantsData->insert("HeaderFont", QVariant(headerFont));

    QFont groupHeaderFont;
    groupHeaderFont.setFamily("Nokia Pure Text");
    groupHeaderFont.setPixelSize(18);
    groupHeaderFont.setBold(true);
    uiConstantsData->insert("GroupHeaderFont", QVariant(groupHeaderFont));

    QFont titleFont;
    titleFont.setFamily("Nokia Pure Text");
    titleFont.setPixelSize(26);
    titleFont.setBold(true);
    uiConstantsData->insert("TitleFont", QVariant(titleFont));

    QFont smallTitleFont;
    smallTitleFont.setFamily("Nokia Pure Text");
    smallTitleFont.setPixelSize(24);
    smallTitleFont.setBold(true);
    uiConstantsData->insert("SmallTitleFont", QVariant(titleFont));

    QFont fieldLabelFont;
    fieldLabelFont.setFamily("Nokia Pure Text Light");
    fieldLabelFont.setPixelSize(22);
    uiConstantsData->insert("FieldLabelFont", QVariant(fieldLabelFont));

    QFont subTitleFont;
    subTitleFont.setFamily("Nokia Pure Text Light");
    subTitleFont.setPixelSize(22);
    uiConstantsData->insert("SubtitleFont", QVariant(subTitleFont));

    return uiConstantsData;
}
