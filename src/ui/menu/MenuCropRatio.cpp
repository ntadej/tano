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

#include "MenuCropRatio.h"

MenuCropRatio::MenuCropRatio(VlcVideoWidget *videoWidget,
                   QWidget *parent)
    : MenuCore(parent),
    _videoWidget(videoWidget)
{
    setTitle(tr("Crop video"));
    setIcon(QIcon(":/icons/24x24/interface.png"));
    actionNext()->setText(tr("Next crop option"));

    QAction *original = new QAction(tr("Original"), this);
    _map1.insert(original, Vlc::Original);
    _map2.insert(Vlc::Original, original);
    connect(original, SIGNAL(triggered()), this, SLOT(apply()));
    addItem(original);

    for (int i = 1; i < Vlc::ratioHuman().size(); i++) {
        QAction *action = new QAction(Vlc::ratioHuman()[i], this);
        _map1.insert(action, Vlc::Ratio(i));
        _map2.insert(Vlc::Ratio(i), action);
        connect(action, SIGNAL(triggered()), this, SLOT(apply()));
        addItem(action);
    }
}

MenuCropRatio::~MenuCropRatio() { }

void MenuCropRatio::apply()
{
    QAction *action = qobject_cast<QAction *>(sender());

    if (action) {
        _videoWidget->setCropRatio(_map1[action]);

        emit value(_map1[action]);
    }
}

void MenuCropRatio::setDefault(const Vlc::Ratio &ratio)
{
    _map2[ratio]->setChecked(true);
}
