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

#include "MenuCrop.h"

MenuCrop::MenuCrop(VlcVideoWidget *videoWidget,
				   QWidget *parent)
	: MenuCore(parent),
	_videoWidget(videoWidget)
{
	setTitle(tr("Crop video"));
	setIcon(QIcon(":/icons/24x24/interface.png"));
	actionNext()->setText(tr("Next crop option"));

	_cropOriginal = new QAction(tr("Original"), this);
	connect(_cropOriginal, SIGNAL(triggered()), _videoWidget, SLOT(setCropOriginal()));
	addItem(_cropOriginal);
	_cropOriginal->setChecked(true);

	QAction *crop16_9 = new QAction("16:9", this);
	connect(crop16_9, SIGNAL(triggered()), _videoWidget, SLOT(setCrop16_9()));
	addItem(crop16_9);

	QAction *crop16_10 = new QAction("16:10", this);
	connect(crop16_10, SIGNAL(triggered()), _videoWidget, SLOT(setCrop16_10()));
	addItem(crop16_10);

	QAction *crop1_85_1 = new QAction("1.85:1", this);
	connect(crop1_85_1, SIGNAL(triggered()), _videoWidget, SLOT(setCrop1_85_1()));
	addItem(crop1_85_1);

	QAction *crop2_21_1 = new QAction("2.21:1", this);
	connect(crop2_21_1, SIGNAL(triggered()), _videoWidget, SLOT(setCrop2_21_1()));
	addItem(crop2_21_1);

	QAction *crop2_35_1 = new QAction("2.35:1", this);
	connect(crop2_35_1, SIGNAL(triggered()), _videoWidget, SLOT(setCrop2_35_1()));
	addItem(crop2_35_1);

	QAction *crop2_39_1 = new QAction("2.39:1", this);
	connect(crop2_39_1, SIGNAL(triggered()), _videoWidget, SLOT(setCrop2_39_1()));
	addItem(crop2_39_1);

	QAction *crop4_3 = new QAction("4:3", this);
	connect(crop4_3, SIGNAL(triggered()), _videoWidget, SLOT(setCrop4_3()));
	addItem(crop4_3);

	QAction *crop5_4 = new QAction("5:4", this);
	connect(crop5_4, SIGNAL(triggered()), _videoWidget, SLOT(setCrop5_4()));
	addItem(crop5_4);

	QAction *crop5_3 = new QAction("5:3", this);
	connect(crop5_3, SIGNAL(triggered()), _videoWidget, SLOT(setCrop5_3()));
	addItem(crop5_3);

	QAction *crop1_1 = new QAction("1:1", this);
	connect(crop1_1, SIGNAL(triggered()), _videoWidget, SLOT(setCrop1_1()));
	addItem(crop1_1);
}

MenuCrop::~MenuCrop() { }
