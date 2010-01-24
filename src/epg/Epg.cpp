#include "Epg.h"

#include <QtDebug>
#include <QTime>

Epg::Epg()
{
	step = 0;
	loader = new EpgLoader();
    timer = new QTimer(this);
    date = QDateTime::currentDateTime();

    connect(timer, SIGNAL(timeout()), this, SLOT(epgNow()));
    connect(loader, SIGNAL(epgDone(QStringList)), this, SLOT(epgSet(QStringList)));
}

Epg::~Epg()
{
	delete loader;
	delete timer;
}

void Epg::getEpg(QString epgP)
{
	if (epgP == "") {
		epgChannel = epgP;
		return;
	}

	epgChannel = epgP;
	if(step==0)
		epgFull = QString("/tv-spored.aspx?chn=" + epgChannel);
	else
		epgFull = QString("/tv-spored.aspx?day=" + QString::number(step+1) + "&chn=" + epgChannel);

	loader->getEpg(true,epgFull);
}

void Epg::refresh()
{
    epgNow();
}

void Epg::reload()
{
    step = 0;
    getEpg(epgChannel);
}

void Epg::epgSetFull(QStringList list)
{
	emit epgDone(step+1, list, date.addDays(step).toString("d.M."));
}

void Epg::epgSet(QStringList list)
{
	if(step == 0) {
		epgList.clear();
		epgList = list;
		epgNow();
	}

	epgSetFull(list);

	if(step < 3) {
		step++;
		getEpg(epgChannel);
	} else
		step = 0;
}

void Epg::epgNow()
{
	epgListNow.clear();
	for(int i = 1;i<epgList.size();i+=3) {
		if(QTime::currentTime() > QTime::fromString(epgList.at(i), "hh:mm") && QTime::currentTime() < QTime::fromString(epgList.at(i+3), "hh:mm")) {
			epgListNow << "<a href=\"" + epgList.at(i+1) + "\">" + epgList.at(i) + " - " + epgList.at(i+2) + "</a>"
					   << "<a href=\"" + epgList.at(i+4) + "\">" + epgList.at(i+3) + " - " + epgList.at(i+5) + "</a>";
			emit epgDone(0,epgListNow,"test");
			return;
		}
	}

	timer->start(60000);
}

void Epg::stop()
{
	loader->stop();
    timer->stop();
    epgList.clear();
}
