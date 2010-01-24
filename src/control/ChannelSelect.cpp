#include "ChannelSelect.h"

#include <QDebug>

ChannelSelect::ChannelSelect(QWidget *parent, QLCDNumber *number, QList<int> l)
	: lcd(number)
{
	num = 1;
	timer = new QTimer(parent);

	number1 = 0;
	number2 = 0;
	number3 = 0;

	lim.clear();
	int tmp;
	for (int i = 0; i < l.size(); ++i) {
	     tmp = l.at(i);
	     lim.append(tmp);
	}
	qDebug() << "Channels:" << lim.size();

	key_0 = new QShortcut(QKeySequence("0"), parent, 0, 0, Qt::ApplicationShortcut);
	key_1 = new QShortcut(QKeySequence("1"), parent, 0, 0, Qt::ApplicationShortcut);
	key_2 = new QShortcut(QKeySequence("2"), parent, 0, 0, Qt::ApplicationShortcut);
	key_3 = new QShortcut(QKeySequence("3"), parent, 0, 0, Qt::ApplicationShortcut);
	key_4 = new QShortcut(QKeySequence("4"), parent, 0, 0, Qt::ApplicationShortcut);
	key_5 = new QShortcut(QKeySequence("5"), parent, 0, 0, Qt::ApplicationShortcut);
	key_6 = new QShortcut(QKeySequence("6"), parent, 0, 0, Qt::ApplicationShortcut);
	key_7 = new QShortcut(QKeySequence("7"), parent, 0, 0, Qt::ApplicationShortcut);
	key_8 = new QShortcut(QKeySequence("8"), parent, 0, 0, Qt::ApplicationShortcut);
	key_9 = new QShortcut(QKeySequence("9"), parent, 0, 0, Qt::ApplicationShortcut);

	connect(key_0, SIGNAL(activated()), this, SLOT(keyPressed_0()));
	connect(key_1, SIGNAL(activated()), this, SLOT(keyPressed_1()));
	connect(key_2, SIGNAL(activated()), this, SLOT(keyPressed_2()));
	connect(key_3, SIGNAL(activated()), this, SLOT(keyPressed_3()));
	connect(key_4, SIGNAL(activated()), this, SLOT(keyPressed_4()));
	connect(key_5, SIGNAL(activated()), this, SLOT(keyPressed_5()));
	connect(key_6, SIGNAL(activated()), this, SLOT(keyPressed_6()));
	connect(key_7, SIGNAL(activated()), this, SLOT(keyPressed_7()));
	connect(key_8, SIGNAL(activated()), this, SLOT(keyPressed_8()));
	connect(key_9, SIGNAL(activated()), this, SLOT(keyPressed_9()));

	connect(timer, SIGNAL(timeout()), this, SLOT(display()));
}

ChannelSelect::~ChannelSelect() {
	delete timer;
	delete key_0;
	delete key_1;
	delete key_2;
	delete key_3;
	delete key_4;
	delete key_5;
	delete key_6;
	delete key_7;
	delete key_8;
	delete key_9;
}

void ChannelSelect::process(int key)
{
	if(num == 1) {
		old = lcd->intValue();
		number1 = key;
	} else if(num == 2) {
		number2 = number1;
		number1 = key;
	} else if(num == 3) {
		number3 = number2;
		number2 = number1;
		number1 = key;
	}

	full = number3*100 + number2*10 + number1;
	lcd->display(full);

	if(num < 3) {
		timer->start(1000);
		num++;
	} else {
		full = number3*100 + number2*10 + number1;
		display();
	}
}

void ChannelSelect::display()
{
	bool signal = false;

	if(lim.contains(full))
		signal = true;

	if(signal == true) {
		emit channelSelect(full);
		qDebug() << "Tano: Key: " << full;
	} else {
		lcd->display(old);
		emit error(tr("No channel with this number"), 2000);
	}

	number1 = 0;
	number2 = 0;
	number3 = 0;
	num = 1;
	timer->stop();
}

void ChannelSelect::channel(bool direction)
{
	bool signal = false;
	int i = 0;

	old = lcd->intValue();
	if(direction) {
		full = old + 1;
		i = 1;
	} else {
		full = old - 1;
		i = -1;
	}

	while(!lim.contains(full)&&full>0&&full<=lim.at(lim.size()-1))
		full+=i;

	display();
}

void ChannelSelect::back()
{
	channel(false);
}

void ChannelSelect::next()
{
	channel(true);
}

//Process link
void ChannelSelect::keyPressed_0()
{
	process(0);
}
void ChannelSelect::keyPressed_1()
{
	process(1);
}
void ChannelSelect::keyPressed_2()
{
	process(2);
}
void ChannelSelect::keyPressed_3()
{
	process(3);
}
void ChannelSelect::keyPressed_4()
{
	process(4);
}
void ChannelSelect::keyPressed_5()
{
	process(5);
}
void ChannelSelect::keyPressed_6()
{
	process(6);
}
void ChannelSelect::keyPressed_7()
{
	process(7);
}
void ChannelSelect::keyPressed_8()
{
	process(8);
}
void ChannelSelect::keyPressed_9()
{
	process(9);
}
