/****************************************************************************
* Version.h: Version info
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

#ifndef TANO_VERSION_H_
#define TANO_VERSION_H_

#include <QtCore/QString>

class Version
{
public:
	static QString version();
	static QString changeset();
	static QString qt();
	static QString libVlcqt();
	static QString libVlc();
};

#endif // TANO_VERSION_H_
