#include <QHeaderView>
#include <QTableWidget>
#include <QDebug>

#include "EpgToday.h"

EpgToday::EpgToday(QWidget *parent)
    : QTableWidget(parent)
{
	this->setColumnCount(1);
	this->horizontalHeader()->setStretchLastSection(true);
	this->verticalHeader()->hide();

	epgClear();

	connect(this, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(epgClicked(QTableWidgetItem*)));
}

EpgToday::~EpgToday()
{

}

void EpgToday::setEpg(QString epg) {
	epgList = epg.split(";");
	this->clear();
	this->setRowCount((epgList.size() - 1)/3 + 1);

	processEpg();
}

void EpgToday::epgClear() {
	this->clear();
	this->setRowCount(0);
	QStringList epgHeader;
	epgHeader << tr("No EPG");
	this->setHorizontalHeaderLabels(epgHeader);
}

void EpgToday::processEpg() {
	QStringList epgHeader;
	epgHeader << epgList.at(0);
	this->setHorizontalHeaderLabels(epgHeader);

	int r = 0;
	for (int i = 1; i < epgList.size(); i+=3) {
		newEpg = new ChannelEpg(epgList.at(i),epgList.at(i+1),epgList.at(i+2));
		newItem = new QTableWidgetItem(QString(epgList.at(i) + " - " + epgList.at(i+2)));
		newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		map.insert(newItem, newEpg);
		this->setItem(r, 0, newItem);
		r++;
	}
}

void EpgToday::epgClicked(QTableWidgetItem *item) {
	ChannelEpg *newEpg = map[item];
	emit urlClicked(newEpg->url());
}
