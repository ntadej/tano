/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak <ntadej@users.sourceforge.net>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <QtCore/QDebug>
#include <QtGui/QTextEdit>

#include "container/EpgDayList.h"
#include "container/EpgItem.h"
#include "container/EpgShowInfo.h"
#include "core/ConsoleOutput.h"
#include "epg/EpgSlovenia.h"

EpgSlovenia::EpgSlovenia() {}

EpgSlovenia::~EpgSlovenia() {}

QString EpgSlovenia::host() const
{
	QString hostUrl("www.siol.net");
	return hostUrl;
}

QHttpRequestHeader EpgSlovenia::httpHeader(const QString &url) const
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

bool EpgSlovenia::init(const QString &input)
{
	QRegExp flag("flag\\s*=\\s*(\\w{10,30})");
	flag.indexIn(input);

	_flag = flag.cap(1);
	if(ConsoleOutput::debug())
		qDebug() << "EPG:" << "Flag:" << _flag;

	if(_flag.size() > 0)
		return true;
	else
		return false;
}

QString EpgSlovenia::load(const QString &input,
								const int &arg) const
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

QString EpgSlovenia::processDate(const QString &input) const
{
	QString date = input;
	date = date.replace(QString(", ponedeljek "), "");
	date = date.replace(QString(", torek "), "");
	date = date.replace(QString(", sreda "), "");
	date = date.replace(QString().fromUtf8(", četrtek "), "");
	date = date.replace(QString(", petek "), "");
	date = date.replace(QString(", sobota "), "");
	date = date.replace(QString(", nedelja "), "");

	return date;
}

EpgDayList EpgSlovenia::processSchedule(const QString &channel,
										const int &day,
										const QString &input) const
{
	EpgDayList schedule(channel, day);
	if(!input.contains("schedule_title")) {
		schedule.setValid(false);
		return schedule;
	}

	QStringList list[3];

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
		schedule << EpgItem(QTime::fromString(list[0][i], "hh:mm"), list[1][i].prepend("http://www.siol.net/"), list[2][i]);

	return schedule;
}

EpgShowInfo EpgSlovenia::processShow(const QString &input) const
{
	if(!input.contains("schedule_title"))
		return EpgShowInfo(false);

	EpgShowInfo info;

	QRegExp exp[9];
	// Title
	exp[0].setPattern("<h4>(.*)</h4>");
	exp[0].indexIn(input);
	info.setTitle(exp[0].cap(1).replace(QRegExp("(<.*>)"), ""));

	// Channel
	exp[1].setPattern("class=\"title\">([^<]*)");
	exp[1].indexIn(input);
	info.setChannel(exp[1].cap(1));

	// Start and end time
	exp[2].setPattern("class=\"time\">([^<]*)");
	exp[3].setPattern("class=\"duration\">([^<]*)</span>");
	exp[2].indexIn(input);
	QString date = processDate(exp[2].cap(1));
	exp[3].indexIn(input);
	QStringList time = exp[3].cap(1).split(" - ");
	info.setStartTime(QDateTime::fromString(QString(date + " " + time[0]), "d.M.yyyy hh:mm"));
	info.setEndTime(QDateTime::fromString(QString(date + " " + time[1]), "d.M.yyyy hh:mm"));

	// Info
	exp[4].setPattern("class=\"sub\">([^<]*)"); //Info
	exp[4].indexIn(input);
	info.setInfo(exp[4].cap(1));

	// Description
	exp[5].setPattern("class=\"desc\">(.*)</p><p class=\"prevnext\"");
	exp[5].indexIn(input);
	if(!exp[5].cap(1).contains("<br /><br />"))
		info.setDescription(exp[5].cap(1).replace("</p>",""));
	else
		info.setDescription(exp[5].cap(1).remove(exp[5].cap(1).size()-12, 12).prepend("<p>"));

	// Image
	exp[6].setPattern("<h4><img src='([^']*)");
	exp[6].indexIn(input);
	info.setImage(exp[6].cap(1));

	// Previous
	exp[7].setPattern("href=\"([^\"]*)\" title=\"Prej");
	exp[7].indexIn(input);
	info.setPrevious(exp[7].cap(1).prepend("http://www.siol.net/").replace("&amp;","&"));

	// Next
	exp[8].setPattern("href=\"([^\"]*)\" title=\"Naslednja");
	exp[8].indexIn(input);
	info.setNext(exp[8].cap(1).prepend("http://www.siol.net/").replace("&amp;","&"));

	return info;
}
