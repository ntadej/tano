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

#include <QtGui/QHBoxLayout>

#include <vlc-qt/Video.h>

#include "Teletext.h"

Teletext::Teletext(QWidget *parent)
	: QWidget(parent)
{
	setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

	_button = new QPushButton(this);
	_button->setCheckable(true);
	_button->setChecked(false);
	_button->setIcon(QIcon(":/icons/48x48/teletext.png"));
	_button->setIconSize(QSize(48, 48));
	_button->setMaximumSize(QSize(48, 48));
	_button->setMinimumSize(QSize(48, 48));
	_button->setToolTip(tr("Teletext"));
	_button->setStatusTip(tr("Teletext"));

	_page = new QSpinBox(this);
	_page->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	_page->setVisible(false);
	_page->setMinimumHeight(48);
	_page->setMinimum(100);
	_page->setMaximum(999);

	connect(_button, SIGNAL(toggled(bool)), _page, SLOT(setVisible(bool)));
	connect(_button, SIGNAL(toggled(bool)), this, SLOT(teletext(bool)));
	connect(_page, SIGNAL(valueChanged(int)), this, SLOT(page(int)));

	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addWidget(_button);
	layout->addWidget(_page);
	layout->setSpacing(0);
	layout->setMargin(0);
	setLayout(layout);
}

Teletext::~Teletext()
{
	delete _button;
	delete _page;
}

void Teletext::page(const int &p)
{
	VlcVideo::setTeletextPage(p);
}

void Teletext::teletext(const bool &on)
{
	VlcVideo::toggleTeletext();
}
