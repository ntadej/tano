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

#include "style/StyledBar.h"
#include "style/StyleHelper.h"

#include <QPainter>
#include <QStyleOption>

StyledBar::StyledBar(QWidget *parent)
    : QWidget(parent)
{
    setProperty("panelwidget", true);
    setProperty("panelwidget_singlerow", true);
    setProperty("lightColored", false);
}

void StyledBar::setSingleRow(bool singleRow)
{
    setProperty("panelwidget_singlerow", singleRow);
}

bool StyledBar::isSingleRow() const
{
    return property("panelwidget_singlerow").toBool();
}

void StyledBar::setLightColored(bool lightColored)
{
    if (isLightColored() == lightColored)
        return;
    setProperty("lightColored", lightColored);
    foreach (QWidget *childWidget, findChildren<QWidget *>())
        childWidget->style()->polish(childWidget);
}

bool StyledBar::isLightColored() const
{
    return property("lightColored").toBool();
}

void StyledBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    QStyleOptionToolBar option;
    option.rect = rect();
    option.state = QStyle::State_Horizontal;
    style()->drawControl(QStyle::CE_ToolBar, &option, &painter, this);
}

StyledSeparator::StyledSeparator(QWidget *parent)
    : QWidget(parent)
{
    setFixedWidth(10);
}

void StyledSeparator::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    QStyleOption option;
    option.rect = rect();
    option.state = QStyle::State_Horizontal;
    option.palette = palette();
    style()->drawPrimitive(QStyle::PE_IndicatorToolBarSeparator, &option, &painter, this);
}

SimpleSeparator::SimpleSeparator(QWidget *parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}
