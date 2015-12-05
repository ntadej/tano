/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
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

#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QHBoxLayout>

#include "OsdFloat.h"

OsdFloat::OsdFloat(QWidget *parent)
    : QFrame(parent)
{
    _slowHideTimer = new QTimer(this);
    _slowShowTimer = new QTimer(this);

    _desktopWidth = QApplication::desktop()->width();
    _desktopHeight = QApplication::desktop()->height();

    _alwaysHide = false;

    setWindowFlags(Qt::ToolTip);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    connect(_slowHideTimer, SIGNAL(timeout()), this, SLOT(slowHide()));
    connect(_slowShowTimer, SIGNAL(timeout()), this, SLOT(slowShow()));
}

OsdFloat::~OsdFloat()
{
    delete _slowHideTimer;
    delete _slowShowTimer;
}

void OsdFloat::floatHide()
{
#if defined(Q_OS_WIN)
    hide();
#else
    _slowHideTimer->start(5);
#endif
}

void OsdFloat::floatShow()
{
#if !defined(Q_OS_WIN)
    _slowHideTimer->stop();

    if (isVisible() && windowOpacity() >= 1)
        return;

    _slowShowTimer->start(5);
#endif
    show();
}

void OsdFloat::setControls()
{
    _defaultHeight = height();
    if (_desktopWidth > 1366) {
        _defaultWidth = 2*1366/3;
    } else {
        _defaultWidth = 2*_desktopWidth/3;
    }

    resize(_defaultWidth, _defaultHeight);

    _defaultX = (_desktopWidth - width())/2;
    _defaultY = _desktopHeight - height();

    move(_defaultX, _defaultY);
}

void OsdFloat::setInfo()
{
    _defaultHeight = 500;
    _defaultWidth = 300;

    resize(_defaultWidth, _defaultHeight);

    _defaultX = _desktopWidth - width();
    _defaultY = (_desktopHeight - height() - 100)/2;

    move(_defaultX, _defaultY);
}

void OsdFloat::setSchedule()
{
    _defaultHeight = 500 + (_desktopHeight - 600) * 0.5;

    _defaultY = (_desktopHeight - _defaultHeight - 100)/2;
    _defaultX = _defaultY;

    if (_desktopWidth - 400 - _defaultX > 700)
        _defaultWidth = 700;
    else
        _defaultWidth = _desktopWidth - 400 - _defaultX;

    resize(_defaultWidth, _defaultHeight);

    _alwaysHide = true;

    move(_defaultX, _defaultY);
}

void OsdFloat::setWidget(QWidget *widget)
{
    layout()->addWidget(widget);
}

void OsdFloat::slowHide()
{
    setWindowOpacity(windowOpacity() - 0.04);

    if (windowOpacity() <= 0) {
        _slowHideTimer->stop();

#if !defined(Qt4) && !defined(Q_WS_X11)
        hide();
#else
        if (_alwaysHide)
            hide();
#endif
    }
}

void OsdFloat::slowShow()
{
    setWindowOpacity(windowOpacity() + 0.08);

    if (windowOpacity() >= 1) {
        _slowShowTimer->stop();
    }
}
