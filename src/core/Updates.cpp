/****************************************************************************
* Updates.cpp: Update manager
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

#include "Updates.h"
#include "Ver.h"

Updates::Updates()
	: _codec(QTextCodec::codecForName("UTF-8"))
{
	setHost("tano.sourceforge.net");
}

Updates::~Updates()
{

}

void Updates::getUpdates()
{
	get("/version.php");
	connect(this, SIGNAL(done(bool)), this, SLOT(updatesPrint()));
}

void Updates::updatesPrint()
{
	QByteArray httpResponse = readAll();

	QString string = _codec->toUnicode(httpResponse);
	string = string.replace(" ","");
	string = string.replace("\n","");

	disconnect(this, SIGNAL(done(bool)), this, SLOT(updatesPrint()));

	processUpdates(string);
}

void Updates::processUpdates(const QString &v)
{
	QStringList updatesList;

	if(Version::Tano() != v) {
		if(Version::Tano().contains("-r"))
			updatesList << "svn" << v << Version::Tano();
		else
			updatesList << "update" << v;
	} else {
		updatesList << "latest";
	}

	emit updatesDone(updatesList);
}
