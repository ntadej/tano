/****************************************************************************
* EpgItem.cpp: Epg item container class
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

#ifndef TANO_EPGITEM_H_
#define TANO_EPGITEM_H_

#include <QtCore/QString>

class EpgItem
{
public:
	EpgItem(const QString &time,
			const QString &url,
			const QString &title);
	~EpgItem();

	QString time() const { return _time; }
	QString url() const { return _url; }
	QString title() const { return _title; }

private:
	QString _time;
	QString _url;
	QString _title;
};

#endif // TANO_EPGITEM_H_
