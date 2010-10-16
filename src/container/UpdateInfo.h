/****************************************************************************
* UpdateInfo.h: Update information container class
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

#ifndef TANO_UPDATEINFO_H_
#define TANO_UPDATEINFO_H_

#include <QtCore/QString>

class UpdateInfo
{
public:
	UpdateInfo(const QString &name = 0);
	~UpdateInfo();

	QString name() const { return _name; }
	void setName(const QString &name) { _name = name; }
	QString version() const { return _version; }
	void setVersion(const QString &version) { _version = version; }
	QString date() const { return _date; }
	void setDate(const QString &date) { _date = date; }
	QString description() const { return _description; }
	void setDescription(const QString &description) { _description = description; }
	bool development() const { return _development;}
	void setDevelopment(const bool &development) { _development = development; }

private:
	QString _name;
	QString _version;
	QString _date;
	QString _description;
	bool _development;
};

#endif // TANO_UPDATEINFO_H_
