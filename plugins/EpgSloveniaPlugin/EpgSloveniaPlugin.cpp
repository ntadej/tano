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
	int m = input.indexOf("flag=", input.indexOf("whatson"));
	int n = input.indexOf("\"",m);

	QString flag;
	for(int i=m+5;i<n;i++)
		flag.append(QString(input[i]));

	_flag = flag;

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
	} else
		epg = epg.remove("http://www.siol.net");

	return epg;
}

QStringList EpgSloveniaPlugin::processSchedule(const QString &input) const
{
	if(!input.contains("schedule_title"))
		return QStringList() << "error";

	int n;
	QString value = input;

	//Main EPG
	n = value.indexOf("<dl class=\"listB\">");
	value.remove(0,n);
	n = value.indexOf("</dl>");
	value.remove(n,value.size()-1-n);

	//Changes
	value.replace("<table class=\"schedule\">", ": ");
	value.replace("(<a href=", "(<p style=");
	value.replace("ogled</a>)","ogled</p>)");
	value.replace("<a", "|<a");
	value.replace("</a>", "|");
	value.replace("	","");
	value.replace(" href=\"",">");

	QTextEdit *edit = new QTextEdit();
	edit->setHtml(value);
	value = edit->toPlainText();
	delete edit;

	value.replace("\n","");
	value.replace("\">","|");
	value.replace("tv-spored.aspx", "http://www.siol.net/tv-spored.aspx");
	value.replace("|: ","|");
	value.replace(".2009: ", ".2009:|");
	value.replace(".2010: ", ".2010:|");
	value.replace(".2011: ", ".2011:|");
	value.replace(" (ogled): ","");
	value.replace(" (ogled)","");
	value.remove(value.size()-1,1);

	return value.split("|");
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
