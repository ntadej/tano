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
		if(arg==0)
			epg = QString("/tv-spored.aspx?chn=" + input);
		else
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

	int n;
	QString tmp;
	QStringList show;
	QTextEdit *edit = new QTextEdit();

	//Title
	tmp = input;
	n = tmp.indexOf("<div class=\"event\">");
		tmp.remove(0,n);
	n = tmp.indexOf("</h4>");
	tmp.remove(n,tmp.size()-1-n);
	edit->setHtml(tmp);
	tmp = edit->toPlainText();
	edit->setText("");
	show << tmp;

	//Time
	tmp = input;
	n = tmp.indexOf("<div class=\"schedule_title\">");
	tmp.remove(0,n);
	n = tmp.indexOf("</div>");
	tmp.remove(n,tmp.size()-1-n);
	edit->setHtml(tmp);
	tmp = edit->toPlainText();
	edit->setText("");
	tmp.replace(" ,",",");
	show << tmp;

	//Info
	tmp = input;
	n = tmp.indexOf("<p class=\"sub\">");
	tmp.remove(0,n);
	n = tmp.indexOf("</p>");
	tmp.remove(n,tmp.size()-1-n);
	edit->setHtml(tmp);
	tmp = edit->toPlainText();
	edit->setText("");
	show << tmp;

	//Description
	tmp = input;
	n = tmp.indexOf("<p class=\"desc\">");
	tmp.remove(0,n);
	n = tmp.indexOf("</p>");
	tmp.remove(n,tmp.size()-1-n);
	edit->setHtml(tmp);
	tmp = edit->toPlainText();
	edit->setText("");
	show << tmp;

	//Actor
	tmp = input;
	n = tmp.indexOf("<p class=\"actor\">");
	tmp.remove(0,n);
	n = tmp.indexOf("</p>");
	tmp.remove(n,tmp.size()-1-n);
	edit->setHtml(tmp);
	tmp = edit->toPlainText();
	edit->setText("");
	tmp.remove(tmp.size()-2,2);
	show << tmp;

	//Image
	tmp = input;
	n = tmp.indexOf("<div class=\"event\">");
	tmp.remove(0,n);
	n = tmp.indexOf("</h4>");
	tmp.remove(n,tmp.size()-1-n);
	if(!tmp.contains("<img src")) {
		tmp = "";
	} else {
		n = tmp.indexOf("<img src='");
		tmp.remove(0,n+10);
		n = tmp.indexOf("' alt=\"\"");
		tmp.remove(n,tmp.size()-1-n);
		tmp.remove(tmp.size()-1,1);
	}
	show << tmp;

	delete edit;

	return show;
}
