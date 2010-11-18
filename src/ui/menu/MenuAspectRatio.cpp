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

#include "MenuAspectRatio.h"

MenuAspectRatio::MenuAspectRatio(VlcVideoWidget *videoWidget,
				   QWidget *parent)
	: MenuCore(parent),
	_videoWidget(videoWidget)
{
	setTitle(tr("Aspect ratio"));
	setIcon(QIcon(":/icons/24x24/interface.png"));
	actionNext()->setText(tr("Next aspect ratio option"));

	_aspectRatioOriginal = new QAction(tr("Original"), this);
	connect(_aspectRatioOriginal, SIGNAL(triggered()), _videoWidget, SLOT(setRatioOriginal()));
	addItem(_aspectRatioOriginal);
	_aspectRatioOriginal->setChecked(true);

	QAction *ratio16_9 = new QAction("16:9", this);
	connect(ratio16_9, SIGNAL(triggered()), _videoWidget, SLOT(setRatio16_9()));
	addItem(ratio16_9);

	QAction *ratio16_10 = new QAction("16:10", this);
	connect(ratio16_10, SIGNAL(triggered()), _videoWidget, SLOT(setRatio16_10()));
	addItem(ratio16_10);

	QAction *ratio2_21_1 = new QAction("2.21:1", this);
	connect(ratio2_21_1, SIGNAL(triggered()), _videoWidget, SLOT(setRatio2_21_1()));
	addItem(ratio2_21_1);

	QAction *ratio4_3 = new QAction("4:3", this);
	connect(ratio4_3, SIGNAL(triggered()), _videoWidget, SLOT(setRatio4_3()));
	addItem(ratio4_3);

	QAction *ratio5_4 = new QAction("5:4", this);
	connect(ratio5_4, SIGNAL(triggered()), _videoWidget, SLOT(setRatio5_4()));
	addItem(ratio5_4);

	QAction *ratio1_1 = new QAction("1:1", this);
	connect(ratio1_1, SIGNAL(triggered()), _videoWidget, SLOT(setRatio1_1()));
	addItem(ratio1_1);
}

MenuAspectRatio::~MenuAspectRatio() { }
