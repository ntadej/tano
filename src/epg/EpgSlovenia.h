/****************************************************************************
* EpgSlovenia.h: EPG info for Slovenia
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

#ifndef TANO_EPGSLOVENIA_H_
#define TANO_EPGSLOVENIA_H_

#include <QtCore/QProcess>
#include <QtCore/QString>
#include <QtNetwork/QHttpRequestHeader>

class EpgSlovenia
{
public:
	EpgSlovenia();
	~EpgSlovenia();

	QString host() const;
	QHttpRequestHeader httpHeader(const QString &url) const;
	bool init(const QString &text);
	QString load(const QString &input,
				 const int &arg = 0) const;
	QStringList processSchedule(const QString &input) const;
	QStringList processShow(const QString &input) const;

private:
	QString _flag;
};

#endif // TANO_EPGSLOVENIA_H_
