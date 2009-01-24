#include <QHeaderView>
#include <QTableWidget>

#include "EpgToday.h"

EpgToday::EpgToday(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.epgTable, SIGNAL(itemActivated(QTableWidgetItem*)), this, SLOT(epgClicked(QTableWidgetItem*)));
}

EpgToday::~EpgToday()
{

}

void EpgToday::setEpg(QString epg) {
	epgList = epg.split(";");
}

void EpgToday::showEpg() {
	ui.epgTable->clear();
	ui.epgTable->setRowCount((epgList.size() - 1)/3);

	processEpg();

	this->show();
}

void EpgToday::processEpg() {
	QStringList epgHeader;
	epgHeader << epgList.at(0);
	ui.epgTable->setHorizontalHeaderLabels(epgHeader);
	ui.epgTable->horizontalHeader()->setStretchLastSection(true);

	int r = 0;
	for (int i = 1; i < epgList.size(); i+=3) {
		newEpg = new ChannelEpg(epgList.at(i),epgList.at(i+1),epgList.at(i+2));
		newItem = new QTableWidgetItem(QString(epgList.at(i) + " - " + epgList.at(i+2)));
		map.insert(newItem, newEpg);
		ui.epgTable->setItem(r, 0, newItem);
		r++;
	}
}

void EpgToday::epgClicked(QTableWidgetItem *item) {
	ChannelEpg *newEpg = map[item];
	emit urlClicked(newEpg->url());
}
