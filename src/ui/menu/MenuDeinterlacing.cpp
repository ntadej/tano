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

#include "MenuDeinterlacing.h"

MenuDeinterlacing::MenuDeinterlacing(VlcVideoWidget *videoWidget,
									 QWidget *parent)
	: MenuCore(parent),
	_videoWidget(videoWidget)
{
	setTitle(tr("Deinterlacing"));
	setIcon(QIcon(":/icons/24x24/video.png"));
	actionNext()->setText(tr("Next deinterlacing option"));

	_dfOriginal = new QAction(tr("Disabled"), this);
	connect(_dfOriginal, SIGNAL(triggered()), _videoWidget, SLOT(setFilterDisabled()));
	addItem(_dfOriginal);
	_dfOriginal->setChecked(true);

	QAction *dfDiscard = new QAction(tr("Discard"), this);
	connect(dfDiscard, SIGNAL(triggered()), _videoWidget, SLOT(setFilterDiscard()));
	addItem(dfDiscard);

	QAction *dfBlend = new QAction(tr("Blend"), this);
	connect(dfBlend, SIGNAL(triggered()), _videoWidget, SLOT(setFilterBlend()));
	addItem(dfBlend);

	QAction *dfMean = new QAction(tr("Mean"), this);
	connect(dfMean, SIGNAL(triggered()), _videoWidget, SLOT(setFilterMean()));
	addItem(dfMean);

	QAction *dfBob = new QAction(tr("Bob"), this);
	connect(dfBob, SIGNAL(triggered()), _videoWidget, SLOT(setFilterBob()));
	addItem(dfBob);

	QAction *dfLinear = new QAction(tr("Linear"), this);
	connect(dfLinear, SIGNAL(triggered()), _videoWidget, SLOT(setFilterLinear()));
	addItem(dfLinear);

	QAction *dfX = new QAction(tr("X"), this);
	connect(dfX, SIGNAL(triggered()), _videoWidget, SLOT(setFilterX()));
	addItem(dfX);
}

MenuDeinterlacing::~MenuDeinterlacing() { }
