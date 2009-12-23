#include <QTableWidgetItem>

#include "Timers.h"

Timers::Timers(QWidget *parent)
	:QMainWindow(parent)
{
	ui.setupUi(this);

	ui.timersList->horizontalHeader()->setStretchLastSection(true);

	//Test
	QTableWidgetItem *newItem = new QTableWidgetItem("Test channel");
	newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
	ui.timersList->setRowCount(1);
	ui.timersList->setItem(0, 0, newItem);
}

Timers::~Timers() {

}
