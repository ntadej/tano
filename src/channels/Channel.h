/****************************************************************************
* Channel.h: Channel class
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

#ifndef TANO_CHANNEL_H_
#define TANO_CHANNEL_H_

#include <QtCore/QString>
#include <QtCore/QStringList>

class Channel
{
public:
	Channel(const QString &name, const int &number);
	~Channel();

	void setNumber(const int &number);
	void setName(const QString &name);
	void setEpg(const QString &epg);
	void setLanguage(const QString &language);
	void setUrl(const QString &url);
	void setCategories(const QStringList &categories);

	int number() const {return _number;};
	QString numberString() const {return QString().number(_number);};
	QString name() const {return _name;};
	QString language() const {return _language;};
	QString url() const {return _url;};
	QString epg() const {return _epg;};
	QStringList categories() const {return _categories;};

private:
	int _number;
	QString _name;
	QString _language;
	QString _url;
	QString _epg;
	QStringList _categories;
};

#endif // TANO_CHANNEL_H_
