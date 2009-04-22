#include "Epg.h"

#include <QtDebug>
#include <QTime>

Epg::Epg()
{
	loader = new EpgLoader();
    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(epgNow()));
    connect(loader, SIGNAL(epgDone(QStringList)), this, SLOT(epgSet(QStringList)));
}

Epg::~Epg()
{

}

void Epg::getEpg(QString epgP)
{
	if (epgP == "NI") {
		epgChannel = epgP;
		return;
	}

	epgChannel = epgP;
	epgFull = QString("/tv-spored.aspx?chn=" + epgChannel);

	loader->getEpg(epgFull);
}

void Epg::refresh()
{
    epgNow();
}

void Epg::reload()
{
    loader->reload();
}

void Epg::epgSet(QStringList list)
{
	epgList.clear();
	epgList = list;

	epgNow();
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

	timer->start(60000);
}

void Epg::stop()
{
	loader->stop();
    timer->stop();
    epgList.clear();
}
