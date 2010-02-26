/****************************************************************************
* EpgSloveniaPlugin.cpp: EPG plugin for Slovenia
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

#ifndef TANO_EPGSLOVENIAPLUGIN_H_
#define TANO_EPGSLOVENIAPLUGIN_H_

#include <QtCore/QProcess>
#include <QtCore/QString>

#include "plugins/EpgPlugins.h"

class EpgSloveniaPlugin : public EpgPlugin
{
public:
	EpgSloveniaPlugin();
	~EpgSloveniaPlugin();

	QString host() const;
	QHttpRequestHeader httpHeader(const QString &url) const;
	bool init(const QString &text);
	QString load(const QString &input, const int &arg = 0) const;
	QStringList processSchedule(const QString &input) const;
	QStringList processShow(const QString &input) const;

private:
	QString _flag;
};

#endif // TANO_EPGSLOVENIAPLUGIN_H_
