/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak <ntadej@users.sourceforge.net>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <QtCore/QEvent>

#include "MenuDeinterlacing.h"

MenuDeinterlacing::MenuDeinterlacing(VlcWidgetVideo *videoWidget,
                                     QWidget *parent)
    : MenuCore(parent),
    _videoWidget(videoWidget)
{
    setTitle(tr("Deinterlacing"));
    setIcon(QIcon(":/icons/mimetypes/16/video-x-generic.png"));
    actionNext()->setText(tr("Next deinterlacing option"));

    QAction *dfDisabled = new QAction(tr("Disabled"), this);
    _map1.insert(dfDisabled, Vlc::Disabled);
    _map2.insert(Vlc::Disabled, dfDisabled);
    connect(dfDisabled, SIGNAL(triggered()), this, SLOT(apply()));
    addItem(dfDisabled);

    QAction *dfDiscard = new QAction(tr("Discard"), this);
    _map1.insert(dfDiscard, Vlc::Discard);
    _map2.insert(Vlc::Discard, dfDiscard);
    connect(dfDiscard, SIGNAL(triggered()), this, SLOT(apply()));
    addItem(dfDiscard);

    QAction *dfBlend = new QAction(tr("Blend"), this);
    _map1.insert(dfBlend, Vlc::Blend);
    _map2.insert(Vlc::Blend, dfBlend);
    connect(dfBlend, SIGNAL(triggered()), this, SLOT(apply()));
    addItem(dfBlend);

    QAction *dfMean = new QAction(tr("Mean"), this);
    _map1.insert(dfMean, Vlc::Mean);
    _map2.insert(Vlc::Mean, dfMean);
    connect(dfMean, SIGNAL(triggered()), this, SLOT(apply()));
    addItem(dfMean);

    QAction *dfBob = new QAction(tr("Bob"), this);
    _map1.insert(dfBob, Vlc::Bob);
    _map2.insert(Vlc::Bob, dfBob);
    connect(dfBob, SIGNAL(triggered()), this, SLOT(apply()));
    addItem(dfBob);

    QAction *dfLinear = new QAction(tr("Linear"), this);
    _map1.insert(dfLinear, Vlc::Linear);
    _map2.insert(Vlc::Linear, dfLinear);
    connect(dfLinear, SIGNAL(triggered()), this, SLOT(apply()));
    addItem(dfLinear);

    QAction *dfX = new QAction(tr("X"), this);
    _map1.insert(dfX, Vlc::X);
    _map2.insert(Vlc::X, dfX);
    connect(dfX, SIGNAL(triggered()), this, SLOT(apply()));
    addItem(dfX);
}

MenuDeinterlacing::~MenuDeinterlacing() { }

void MenuDeinterlacing::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        setTitle(tr("Deinterlacing"));
        actionNext()->setText(tr("Next deinterlacing option"));
        _map2[Vlc::Disabled]->setText(tr("Disabled"));
        break;
    default:
        break;
    }
}

void MenuDeinterlacing::apply()
{
    QAction *action = qobject_cast<QAction *>(sender());

    if (action) {
        _videoWidget->setDeinterlacing(_map1[action]);

        emit value(_map1[action]);
    }
}

void MenuDeinterlacing::setDefault(const Vlc::Deinterlacing &deinterlacing)
{
    _map2[deinterlacing]->setChecked(true);
}
