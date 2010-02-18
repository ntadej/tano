/****************************************************************************
* tanohandler.h: Old reader and handler of tano playlist format
* This file is also part of the example classes of the Qt Toolkit.
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

#ifndef TANO_TANOHANDLER_H_
#define TANO_TANOHANDLER_H_

#include <QtCore/QMap>
#include <QtXml/QXmlDefaultHandler>
#include "../channels/Channel.h"

class TanoHandler : public QXmlDefaultHandler
{
public:
	TanoHandler();

    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    bool characters(const QString &str);
    bool fatalError(const QXmlParseException &exception);
    QString errorString() const;

	QString name() const {return playlistName;};
	QList<Channel*> channelList() const {return _channelList;};

private:
	int num;
	int cat;

	Channel *createChannel(const QString &name, const int &num, const bool &cat);
	int channelNumSync(const int &c);

    Channel *channel;
    QString currentText;
    QString errorStr;
    bool metTanoTag;

	QList<Channel*> _channelList;

	QString playlistName;
    QString category;
};

#endif // TANO_TANOHANDLER_H_
