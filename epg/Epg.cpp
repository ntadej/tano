#include "Epg.h"

#include <QtDebug>

Epg::Epg() {

	qDebug() << "EPG Success!";

	step = 0;

	setHost("siol-tv.pfusion.co.cc");

    timer = new QTimer(this);
	codec = QTextCodec::codecForName("UTF-8");

    connect(timer, SIGNAL(timeout()), this, SLOT(epg()));
}

Epg::~Epg() {

}


void Epg::getEpg(QString epgP)
{
	if (epgP == "NI")
		return;

	epgSource = QString("/on-air.php?id=" + epgP);
	epgUrl = QString("/on-air.php?id=" + epgP + "&url=true");
	epgFull = QString("/on-air-full.php?id=" + epgP);
	epgToday();
}

void Epg::refresh()
{
    epgToday();
}


void Epg::epg()
{
	if (step == 1)
		get(epgSource);
	else
		get(epgUrl);

	connect(this, SIGNAL(done(bool)), this, SLOT(epgPrint()));
}

void Epg::epgToday()
{
	get(epgFull);

	connect(this, SIGNAL(done(bool)), this, SLOT(epgPrintToday()));
}

void Epg::epgPrint()
{
	QByteArray httpResponse = readAll();

	QString host = "http://siol-tv.pfusion.co.cc/";

	QString string = codec->toUnicode(httpResponse);
	if (step == 1)
		epgValue = string;
	else
		epgValue = QString("<a href=\"" + host + string + "\">" + epgValue + "</a>");

	disconnect(this, SIGNAL(done(bool)), this, SLOT(epgPrint()));

	if (step == 1) {
		emit epgDone(epgValue, false);
		step = 2;
		epg();
	} else {
		emit epgDone(epgValue, false);
		step = 1;
		timer->start(60000);
	}
}

void Epg::epgPrintToday()
{
	QByteArray httpResponse = readAll();

	QString string = codec->toUnicode(httpResponse);
	epgValue = string.replace(0, 2, "");
	epgValue = epgValue.replace(".2009: ", ".2009:;");
	epgValue = epgValue.replace(" //  ", ";");
	epgValue = epgValue.replace(" // ", ";");

	disconnect(this, SIGNAL(done(bool)), this, SLOT(epgPrintToday()));

	emit epgDone(epgValue.replace(-5, 5, ""), true);

	step = 1;
	epg();
}
