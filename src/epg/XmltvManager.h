/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef TANO_XMLTVMANAGER_H_
#define TANO_XMLTVMANAGER_H_

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "core/Enums.h"

class XmltvChannel;
class XmltvHandler;
class XmltvList;
class XmltvProgramme;

class XmltvManager : public QObject
{
Q_OBJECT
public:
	XmltvManager(QObject *parent = 0);
	~XmltvManager();

public slots:
	void request(const QString &id,
				 const Tano::Id &identifier);
	void requestProgramme(const QString &programme);
	void requestProgrammeNext(XmltvProgramme *programme);
	void requestProgrammePrevious(XmltvProgramme *programme);
	void stop();

signals:
	void epgCurrent(const QString,
					const QString);
	void epgSchedule(XmltvChannel *,
					 const Tano::Id);
	void epgProgramme(XmltvProgramme *);

private slots:
	void current();

private:
	void loadXmltv();
	QString processCurrentString(XmltvProgramme *programme) const;

	Tano::Id _currentIdentifier;
	QString _currentXmltvId;

	XmltvHandler *_handler;
	QTimer *_timer;
	XmltvList *_xmltv;
};

#endif // TANO_XMLTVMANAGER_H_
