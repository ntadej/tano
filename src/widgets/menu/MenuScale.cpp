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

#include <QtCore/QEvent>

#include "MenuScale.h"

MenuScale::MenuScale(VlcWidgetVideo *videoWidget,
                                 QWidget *parent)
    : MenuCore(parent),
    _videoWidget(videoWidget)
{
    setTitle(tr("Scale/Zoom"));
    setIcon(QIcon(":/icons/apps/16/preferences-system-windows.png"));
    actionNext()->setText(tr("Next scale/zoom option"));
    setType(Vlc::Other);

    QAction *original = new QAction(tr("Original"), this);
    _map1.insert(original, Vlc::NoScale);
    _map2.insert(Vlc::NoScale, original);
    connect(original, SIGNAL(triggered()), this, SLOT(apply()));
    addItem(original);

    for (int i = 1; i < Vlc::scale().size(); i++) {
        QAction *action = new QAction(QString::number(Vlc::scale()[i]), this);
        _map1.insert(action, Vlc::Scale(i));
        _map2.insert(Vlc::Scale(i), action);
        connect(action, SIGNAL(triggered()), this, SLOT(apply()));
        addItem(action);
    }
}

MenuScale::~MenuScale() { }

void MenuScale::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        setTitle(tr("Scale/Zoom"));
        actionNext()->setText(tr("Next scale/zoom option"));
        _map2[Vlc::NoScale]->setText(tr("Original"));
        break;
    default:
        break;
    }
}

void MenuScale::apply()
{
    QAction *action = qobject_cast<QAction *>(sender());

    if (action) {
        _videoWidget->setScale(_map1[action]);

        emit value(_map1[action]);
    }
}

void MenuScale::setDefault(const Vlc::Scale &scale)
{
    _map2[scale]->setChecked(true);
}
