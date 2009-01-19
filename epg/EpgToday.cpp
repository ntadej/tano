#include <QHeaderView>

#include "EpgToday.h"

EpgToday::EpgToday(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
}

EpgToday::~EpgToday()
{

}

void EpgToday::setEpg(QString epg) {
	epgList = epg.split(";");
}

void EpgToday::showEpg() {
	ui.epgTable->clear();
	ui.epgTable->setRowCount(epgList.size() - 1);

	QStringList epgHeader;
	epgHeader << epgList.at(0);
	ui.epgTable->setHorizontalHeaderLabels(epgHeader);
	ui.epgTable->horizontalHeader()->setStretchLastSection(true);

	for (int i = 1; i < epgList.size(); i++) {
		QTableWidgetItem *newTime = new QTableWidgetItem(epgList.at(i));
		ui.epgTable->setItem(i-1, 0, newTime);
	}

	this->exec();
}
