#include "EpgLoader.h"

#include <QtDebug>
#include <QTime>

EpgLoader::EpgLoader()
{
	init = false;
	setHost("www.siol.net");
	codec = QTextCodec::codecForName("UTF-8");
	edit = new QTextEdit();
    epgInit();
}

EpgLoader::~EpgLoader()
{

}

void EpgLoader::getEpg(bool f, QString epgP)
{
	epgGet = epgP;

	if(f)
		epgFull = epgP  + "&flag=" + epgFlag;
	else
		epgFull = epgP.remove("http://www.siol.net");

	full = f;

	if(init)
		epg();
}

void EpgLoader::stop()
{
	disconnect(this, SIGNAL(done(bool)), this, SLOT(epgPrint()));
	disconnect(this, SIGNAL(done(bool)), this, SLOT(epgShow()));
}

void EpgLoader::epgInit()
{
	get("/tv-spored.aspx");
	connect(this, SIGNAL(done(bool)), this, SLOT(epgInitDone()));
}

void EpgLoader::epgInitDone()
{
	int m = 0;
	int n = 0;
	QByteArray httpResponse = readAll();

	epgValue = codec->toUnicode(httpResponse);

	m = epgValue.indexOf("flag=",epgValue.indexOf("channels"));
	n = epgValue.indexOf("\" title",m);

	for(int i=m+5;i<n;i++)
		epgFlag.append(epgValue.at(i));

	qDebug() << epgFlag;

	disconnect(this, SIGNAL(done(bool)), this, SLOT(epgInitDone()));

	init = true;
	getEpg(true, epgGet);
}

void EpgLoader::epg()
{
	header = QHttpRequestHeader("GET",epgFull);
	header.setValue("Referer","http://www.siol.net/tv-spored.aspx");
	header.setValue("Host","www.siol.net");

	request(header);

	if(full)
		connect(this, SIGNAL(done(bool)), this, SLOT(epgPrint()));
	else
		connect(this, SIGNAL(done(bool)), this, SLOT(epgShow()));
}

void EpgLoader::epgPrint()
{
	int n = 0;
	QByteArray httpResponse = readAll();

	epgValue = codec->toUnicode(httpResponse);

	if(!epgValue.contains("schedule_title")) {
		disconnect(this, SIGNAL(done(bool)), this, SLOT(epgPrint()));
		return;
	}

	//Main EPG
	n = epgValue.indexOf("<dl class=\"listB\">");
	epgValue.remove(0,n);
	n = epgValue.indexOf("</dl>");
	epgValue.remove(n,epgValue.size()-1-n);

	//Changes
	epgValue.replace("<table class=\"schedule\">", ": ");
	epgValue.replace("(<a href=", "(<p style=");
	epgValue.replace("ogled</a>)","ogled</p>)");
	epgValue.replace("<a", ";<a");
	epgValue.replace("</a>", ";");
	epgValue.replace("	","");
	epgValue.replace(" href=\"",">");

	edit->setHtml(epgValue);
	epgValue = edit->toPlainText();
	edit->setText("");

	epgValue.replace("\n","");
	epgValue.replace("\">",";");
	epgValue.replace("tv-spored.aspx", "http://www.siol.net/tv-spored.aspx");
	epgValue.replace(";: ",";");
	epgValue.replace(".2009: ", ".2009:;");
	epgValue.replace(" (ogled): ","");
	epgValue.replace(" (ogled)","");
	epgValue.remove(epgValue.size()-1,1);

	disconnect(this, SIGNAL(done(bool)), this, SLOT(epgPrint()));

	epgList = epgValue.split(";");
	emit epgDone(epgList);
}

void EpgLoader::epgShow()
{
	epgListShow.clear();
	int n = 0;
	QString tmp;
	QByteArray httpResponse = readAll();

	epgValue = codec->toUnicode(httpResponse);

	if(!epgValue.contains("schedule_title")) {
		disconnect(this, SIGNAL(done(bool)), this, SLOT(epgPrint()));
		return;
	}

	//Title
	tmp = epgValue;
	n = tmp.indexOf("<div class=\"event\">");
	tmp.remove(0,n);
	n = tmp.indexOf("</h4>");
	tmp.remove(n,tmp.size()-1-n);
	edit->setHtml(tmp);
	tmp = edit->toPlainText();
	edit->setText("");
	epgListShow << tmp;

	//Time
	tmp = epgValue;
	n = tmp.indexOf("<div class=\"schedule_title\">");
	tmp.remove(0,n);
	n = tmp.indexOf("</div>");
	tmp.remove(n,tmp.size()-1-n);
	edit->setHtml(tmp);
	tmp = edit->toPlainText();
	edit->setText("");
	tmp.replace(" ,",",");
	epgListShow << tmp;

	//Info
	tmp = epgValue;
	n = tmp.indexOf("<p class=\"sub\">");
	tmp.remove(0,n);
	n = tmp.indexOf("</p>");
	tmp.remove(n,tmp.size()-1-n);
	edit->setHtml(tmp);
	tmp = edit->toPlainText();
	edit->setText("");
	epgListShow << tmp;

	//Description
	tmp = epgValue;
	n = tmp.indexOf("<p class=\"desc\">");
	tmp.remove(0,n);
	n = tmp.indexOf("</p>");
	tmp.remove(n,tmp.size()-1-n);
	edit->setHtml(tmp);
	tmp = edit->toPlainText();
	edit->setText("");
	epgListShow << tmp;

	//Actor
	tmp = epgValue;
	n = tmp.indexOf("<p class=\"actor\">");
	tmp.remove(0,n);
	n = tmp.indexOf("</p>");
	tmp.remove(n,tmp.size()-1-n);
	edit->setHtml(tmp);
	tmp = edit->toPlainText();
	edit->setText("");
	tmp.remove(tmp.size()-2,2);
	epgListShow << tmp;

	//Image
	tmp = epgValue;
	n = tmp.indexOf("<div class=\"event\">");
	tmp.remove(0,n);
	n = tmp.indexOf("</h4>");
	tmp.remove(n,tmp.size()-1-n);
	if(!tmp.contains("<img src")) {
		tmp = "NI";
	} else {
		n = tmp.indexOf("<img src='");
		tmp.remove(0,n+10);
		n = tmp.indexOf("' alt=\"\"");
		tmp.remove(n,tmp.size()-1-n);
		tmp.remove(tmp.size()-1,1);
	}
	epgListShow << tmp;

	disconnect(this, SIGNAL(done(bool)), this, SLOT(epgShow()));

	emit epgDone(epgListShow);
}
