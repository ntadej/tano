#include <QHBoxLayout>

#include "VlcSeekWidget.h"
#include "VlcInstance.h"

VlcSeekWidget::VlcSeekWidget(QWidget *parent)
	:QWidget(parent)
{
	seek = new QSlider(this);
	seek->setOrientation(Qt::Horizontal);
	//seek->setMaximum(200);

	labelElapsed = new QLabel(this);
	labelElapsed->setText("00:00:00");

	labelFull = new QLabel(this);
	labelFull->setText("00:00:00");

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(labelElapsed);
	layout->addWidget(seek);
	layout->addWidget(labelFull);
	setLayout(layout);

	timer = new QTimer(this);

	connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
	connect(seek, SIGNAL(valueChanged(int)), this, SLOT(changeTime(int)));

	timer->start(100);
}

VlcSeekWidget::~VlcSeekWidget() {

}

void VlcSeekWidget::updateTime()
{

}

void VlcSeekWidget::changeTime(int newTime)
{

}
