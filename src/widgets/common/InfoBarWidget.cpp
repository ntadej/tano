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

#include "InfoBarWidget.h"

InfoBarWidget::InfoBarWidget(QWidget *parent)
    : ScrollingWidget(parent)
{
    _spacer = " &bull; ";
}

InfoBarWidget::~InfoBarWidget() { }

void InfoBarWidget::clear()
{
    _info = "";
    _epg = "";

    setText();
}

void InfoBarWidget::setChannelEpg(const QString &now,
                                  const QString &next)
{
    _epg = QString("<i>%1</i> %2" + _spacer + "<i>%3</i> %4").arg(tr("Now:"), now, tr("Next:"), next);

    setText(_info + _spacer + _epg);
}

void InfoBarWidget::setChannelInfo(const QString &name,
                                   const QString &language)
{
    if (!language.isEmpty()) {
        _info = QString("<b>%1</b>" + _spacer + "<i>%2</i> %3").arg(name, tr("Language:"), language);
    } else {
        _info = QString("<b>%1</b>").arg(name);
    }

    if (!_epg.isEmpty())
        setText(_info + _spacer + _epg);
    else
        setText(_info);
}

void InfoBarWidget::setRecordingInfo(const QString &name,
                                     const QString &info)
{
    if (!info.isEmpty()) {
        _info = QString("<b>%1</b>" + _spacer + "%2").arg(name,  info);
    } else {
        _info = QString("<b>%1</b>").arg(name);
    }

    setText(_info);
}
