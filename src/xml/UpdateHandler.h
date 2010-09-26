/****************************************************************************
* UpdateHandler.h: Reader and handler of tano updates
* This file is also part of the example classes of the Qt Toolkit.
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
*
* Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
* Contact: Qt Software Information (qt-info@nokia.com)
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

#ifndef TANO_UPDATEHANDLER_H_
#define TANO_UPDATEHANDLER_H_

#include <QtXml/QXmlDefaultHandler>

#include "core/UpdateInfo.h"

class UpdateHandler : public QXmlDefaultHandler
{
public:
	UpdateHandler();
	~UpdateHandler();

	bool startElement(const QString &namespaceURI, const QString &localName,
					  const QString &qName, const QXmlAttributes &attributes);
	bool endElement(const QString &namespaceURI, const QString &localName,
					const QString &qName);
	bool characters(const QString &str);
	QString errorString() const { return _errorStr; }

	QList<UpdateInfo> updateInfo() { return _update; }

private:
	QString _currentText;
	QString _errorStr;
	bool _metTanoTag;
	int _currentId;

	QList<UpdateInfo> _update;
};

#endif // TANO_UPDATEHANDLER_H_
