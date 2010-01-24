#include <QHeaderView>
#include <QTableWidget>
#include <QDebug>

#include "EpgList.h"

EpgList::EpgList(QWidget *parent)
    : QTableWidget(parent)
{
	this->setColumnCount(1);
	this->horizontalHeader()->setStretchLastSection(true);
	this->verticalHeader()->hide();

	epgClear();

	rightMenu = new QMenu();

	info = new QAction(QIcon(":/icons/images/schedule.png"), tr("Show information"), this);
	record = new QAction(QIcon(":/icons/images/record.png"), tr("Record"), this);
	rightMenu->addAction(info);
	rightMenu->addAction(record);

    connect(info, SIGNAL(triggered()), this, SLOT(epgClicked()));
}

EpgList::~EpgList()
{
	delete info;
	delete record;
	delete rightMenu;
}

void EpgList::mouseReleaseEvent(QMouseEvent *event)
{
	if(event->button() == Qt::RightButton) {
		event->ignore();
		rightMenu->exec(event->globalPos());
	} else {
		epgClicked(currentItem());
	}
}

void EpgList::setEpg(QStringList epg) {
	epgList.empty();
	epgList = epg;
	this->clear();
	this->setRowCount((epgList.size() - 1)/3 + 1);

	processEpg();
}

void EpgList::epgClear() {
	this->clear();
	this->setRowCount(0);
	QStringList epgHeader;
	epgHeader << tr("No EPG");
	this->setHorizontalHeaderLabels(epgHeader);
}

void EpgList::processEpg() {
	QStringList epgHeader;
	epgHeader << epgList.at(0);
	this->setHorizontalHeaderLabels(epgHeader);

	int r = 0;
	for (int i = 1; i < epgList.size(); i+=3) {
		ChannelEpg *newEpg = new ChannelEpg(epgList.at(i),epgList.at(i+1),epgList.at(i+2));
		QTableWidgetItem *newItem = new QTableWidgetItem(QString(epgList.at(i) + " - " + epgList.at(i+2)));
		newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		map.insert(newItem, newEpg);
		this->setItem(r, 0, newItem);
		r++;
	}
}

void EpgList::epgClicked(QTableWidgetItem *item) {
	if(item == 0)
		emit urlClicked(map[currentItem()]->url());
	else
		emit urlClicked(map[item]->url());
}
