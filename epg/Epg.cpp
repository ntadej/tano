#include "Epg.h"

#include <QtDebug>
#include <QTime>

Epg::Epg() {

	init = false;

	qDebug() << "EPG Success!";

	setHost("www.siol.net");

    timer = new QTimer(this);
	codec = QTextCodec::codecForName("UTF-8");
	edit = new QTextEdit();

    connect(timer, SIGNAL(timeout()), this, SLOT(epg()));

    epgInit();
}

Epg::~Epg() {

}

void Epg::getEpg(QString epgP)
{
	if (epgP == "NI") {
		epgChannel = epgP;
		return;
	}

	epgChannel = epgP;
	epgFull = QString("/tv-spored.aspx?chn=" + epgChannel + "&flag=" + epgFlag);

	if(init)
		epg();
}

void Epg::refresh()
{
    epg();
}

void Epg::epgInit()
{
	get("/tv-spored.aspx");

	connect(this, SIGNAL(done(bool)), this, SLOT(epgInitDone()));
}

void Epg::epgInitDone()
{
	int n = 0;
	QByteArray httpResponse = readAll();

	epgValue = codec->toUnicode(httpResponse);

	n = epgValue.indexOf("flag=",epgValue.indexOf("channels"));

	for(int i=n+5;i<n+25;i++)
		epgFlag.append(epgValue.at(i));

	disconnect(this, SIGNAL(done(bool)), this, SLOT(epgInitDone()));

	init = true;
	getEpg(epgChannel);
}

void Epg::epg()
{
	get(epgFull);

	connect(this, SIGNAL(done(bool)), this, SLOT(epgPrint()));
}

void Epg::epgPrint()
{
	int n = 0;
	QByteArray httpResponse = readAll();

	epgValue = codec->toUnicode(httpResponse);

	if(!epgValue.contains("schedule_title")) {
		disconnect(this, SIGNAL(done(bool)), this, SLOT(epgPrint()));
		timer->start(100);
		return;
	}

	//Main EPG
	n = epgValue.indexOf("<dl class=\"listB\">");
	epgValue.remove(0,n);
	n = epgValue.indexOf("</dl>");
	epgValue.remove(n,epgValue.size()-1-n);

	//Changes
	epgValue.replace("<table class=\"schedule\">", ": ");
	epgValue.replace("<a", ";<a");
	epgValue.replace("</a>", ";");
	epgValue.replace("	","");
	epgValue.replace(" href=\"",">");

	edit->setHtml(epgValue);
	epgValue = edit->toPlainText();
	edit->setText("");

	epgValue.replace("\n","");
	epgValue.replace("\">",";");
	epgValue.replace("tv-spored.aspx?val=", "http://tano.pfusion.co.cc/siol/epg/" + epgChannel + "/");
	epgValue.replace("&flag=", "/");
	epgValue.replace(";: ",";");
	epgValue.replace(".2009: ", ".2009:;");
	epgValue.remove(epgValue.size()-1,1);

	disconnect(this, SIGNAL(done(bool)), this, SLOT(epgPrint()));

	epgList = epgValue.split(";");
	emit epgDoneFull(epgList);

	epgNow();

	timer->start(60000);
}

void Epg::epgNow()
{
	epgListNow.clear();
	for(int i = 1;i<epgList.size();i+=3) {
		if(QTime::currentTime() > QTime::fromString(epgList.at(i), "hh:mm") && QTime::currentTime() < QTime::fromString(epgList.at(i+3), "hh:mm")) {
			epgListNow << "<a href=\"" + epgList.at(i+1) + "\">" + epgList.at(i) + " - " + epgList.at(i+2) + "</a>"
					   << "<a href=\"" + epgList.at(i+4) + "\">" + epgList.at(i+3) + " - " + epgList.at(i+5) + "</a>";
			emit epgDone(epgListNow);
			return;
		}
	}
}

void Epg::stop()
{
    timer->stop();
    disconnect(this, SIGNAL(done(bool)), this, SLOT(epgPrint()));
}
