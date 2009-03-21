#include <QApplication>
#include <QDesktopWidget>

#include "TanoOsd.h"

TanoOsd::TanoOsd(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	this->setWindowFlags(Qt::ToolTip);

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(hideOsd()));
}

TanoOsd::~TanoOsd()
{

}

void TanoOsd::showOsd()
{
	int w = QApplication::desktop()->width();
	int h = QApplication::desktop()->height();
	timer->start(1000);
	this->show();
	this->move(w/2-this->width()/2,h-this->height());
}

void TanoOsd::hideOsd()
{
	timer->stop();
	this->hide();
}
