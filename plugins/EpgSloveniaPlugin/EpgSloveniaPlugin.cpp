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

#include <QtCore/QDebug>
#include <QtGui/QTextEdit>

#include "EpgSloveniaPlugin.h"

EpgSloveniaPlugin::EpgSloveniaPlugin()
{
}

EpgSloveniaPlugin::~EpgSloveniaPlugin()
{
}

QString EpgSloveniaPlugin::host() const
{
	return QString("www.siol.net");
}

QHttpRequestHeader EpgSloveniaPlugin::httpHeader(const QString &url) const
{
	QHttpRequestHeader header;
	if(url == "init")
		header = QHttpRequestHeader("GET","/tv-spored.aspx");
	else
		header = QHttpRequestHeader("GET",url);
	header.setValue("Referer","http://www.siol.net/tv-spored.aspx");
	header.setValue("User-Agent","Firefox");
	header.setValue("Host","www.siol.net");

	return header;
}

bool EpgSloveniaPlugin::init(const QString &input)
{
	QRegExp flag("flag\\s*=\\s*(\\w{10,30})");
	flag.indexIn(input);

	_flag = flag.cap(1);
	qDebug() << "EPG: Flag:" << _flag;

	if(_flag.size()>0)
		return true;
	else
		return false;
}

QString EpgSloveniaPlugin::load(const QString &input, const int &arg) const
{
	QString epg = input;

	if(!epg.contains("http://", Qt::CaseInsensitive)) {
		epg = QString("/tv-spored.aspx?day=" + QString::number(arg+1) + "&chn=" + input);

		if(!epg.contains("flag"))
			epg = epg + "&flag=" + _flag;
	} else {
		epg = epg.remove("http://www.siol.net");
	}

	return epg;
}

QStringList EpgSloveniaPlugin::processSchedule(const QString &input) const
{
	if(!input.contains("schedule_title"))
		return QStringList() << "error";

	QStringList mainList;
	QStringList list[3];

	QRegExp title("<div class=\"schedule_title\">\\t*\\s*<span class=\"title\">([^</span>]*)</span>\\t*\\s*<span class=\"time\">([^<]*)");
	title.indexIn(input);
	mainList << title.cap(1)+title.cap(2);

	QRegExp exp[3];
	exp[0].setPattern("class=\"time\">(\\d{2,2}.\\d{2,2})"); // Time
	exp[1].setPattern("href=\"([^\">]*)"); // Url
	exp[2].setPattern(_flag+"\">([^<]*)"); // Name

	int pos;
	int start = input.indexOf("<dd class=\"lowL\">");
	int end = input.indexOf("</dd>", start);

	for(int i=0; i<3; i++) {
		pos = start;
		while ((pos = exp[i].indexIn(input, pos)) != -1 && pos <= end) {
			list[i] << exp[i].cap(1);
			pos += exp[i].matchedLength();
		}
	}

	for(int i=0; i<list[0].size(); i++)
		mainList << list[0][i] << list[1][i].prepend("http://www.siol.net/") << list[2][i];

	return mainList;
}

QStringList EpgSloveniaPlugin::processShow(const QString &input) const
{
	if(!input.contains("schedule_title"))
		return QStringList() << "error";

	QStringList show;

	QRegExp exp[9];
	exp[0].setPattern("<h4>(.*)</h4>"); //Title
	exp[1].setPattern("class=\"title\">([^<]*)"); //Channel
	exp[2].setPattern("class=\"time\">([^<]*)"); //Time
	exp[3].setPattern("class=\"duration\">([^<]*)</span>"); //Duration
	exp[4].setPattern("class=\"sub\">([^<]*)"); //Info
	exp[5].setPattern("class=\"desc\">(.*)</p><p class=\"prevnext\""); //Description
	exp[6].setPattern("<h4><img src='([^']*)"); //Image
	exp[7].setPattern("href=\"([^\"]*)\" title=\"Prej"); //Previous
	exp[8].setPattern("href=\"([^\"]*)\" title=\"Naslednja"); //Next

	for(int i=0; i<9; i++) {
		exp[i].indexIn(input);
		if(i==0)
			show << exp[i].cap(1).replace(QRegExp("(<.*>)"), "");
		else if(i==5)
			if(!exp[i].cap(1).contains("<br /><br />"))
				show << exp[i].cap(1).replace("</p>","");
		else if(i==7 || i==8)
			show << exp[i].cap(1).prepend("http://www.siol.net/");
		else
			show << exp[i].cap(1);
	}

	return show;
}
