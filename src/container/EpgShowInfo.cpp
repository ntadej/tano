/****************************************************************************
* EpgShowInfo.cpp: Epg show information container class
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* This file may be used under the terms of the
* GNU General Public License version 3.0 as published by the
* Free Software Foundation and appearing in the file LICENSE.GPL
* included in the packaging of this file.
*****************************************************************************/

#include "container/EpgShowInfo.h"

EpgShowInfo::EpgShowInfo(const QString &title)
	: _title(title)
{
	_channel = "";
	_info = "";
	_description = "";
	_image = "";
	_previous = "";
	_next = "";
}

EpgShowInfo::~EpgShowInfo() { }
