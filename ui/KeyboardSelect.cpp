#include "KeyboardSelect.h"

#include <QDebug>

KeyboardSelect::KeyboardSelect(QWidget *parent, QLCDNumber *number, QList<int> l) {
	num = 1;
	lcd = number;
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
	qDebug() << "Limit" << lim.size();

	key_0 = new QShortcut(QKeySequence("0"), parent);
	key_1 = new QShortcut(QKeySequence("1"), parent);
	key_2 = new QShortcut(QKeySequence("2"), parent);
	key_3 = new QShortcut(QKeySequence("3"), parent);
	key_4 = new QShortcut(QKeySequence("4"), parent);
	key_5 = new QShortcut(QKeySequence("5"), parent);
	key_6 = new QShortcut(QKeySequence("6"), parent);
	key_7 = new QShortcut(QKeySequence("7"), parent);
	key_8 = new QShortcut(QKeySequence("8"), parent);
	key_9 = new QShortcut(QKeySequence("9"), parent);

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

KeyboardSelect::~KeyboardSelect() {

}

void KeyboardSelect::process(int key)
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
		display();
	}
}

void KeyboardSelect::display()
{
	bool signal = false;
	full = number3*100 + number2*10 + number1;

	int i = 0;
	while(i < lim.size() && signal == false) {
		if(full>=lim.at(i) && full<=lim.at(i+1))
			signal = true;
		i+=2;
	}

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

void KeyboardSelect::channel(bool direction)
{
	bool signal = false;
	int i = 0;

	old = lcd->intValue();
	if(direction)
		full = old + 1;
	else
		full = old - 1;

	while(i < lim.size() && signal == false) {
		if(full>=lim.at(i) && full<=lim.at(i+1)) {
			signal = true;
			emit channelSelect(full);
		}
		i+=2;
	}

	i = 0;
	if(!signal) {
		if(direction) {
			int k = lim.size() - 1;
			if(full < lim.at(k)) {
				while(k > 0 && signal == false) {
					k-=2;
					if(full > lim.at(k)) {
						emit channelSelect(lim.at(k+1));
						signal = true;
					}
				}
			}
		} else {
			while(i < lim.size() && signal == false) {
				if(full < lim.at(i) && full > 0) {
					emit channelSelect(lim.at(i-1));
					signal = true;
				}
				i+=2;
			}
		}
	}
}

void KeyboardSelect::back()
{
	channel(false);
}

void KeyboardSelect::next()
{
	channel(true);
}

//Process link
void KeyboardSelect::keyPressed_0()
{
	process(0);
}
void KeyboardSelect::keyPressed_1()
{
	process(1);
}
void KeyboardSelect::keyPressed_2()
{
	process(2);
}
void KeyboardSelect::keyPressed_3()
{
	process(3);
}
void KeyboardSelect::keyPressed_4()
{
	process(4);
}
void KeyboardSelect::keyPressed_5()
{
	process(5);
}
void KeyboardSelect::keyPressed_6()
{
	process(6);
}
void KeyboardSelect::keyPressed_7()
{
	process(7);
}
void KeyboardSelect::keyPressed_8()
{
	process(8);
}
void KeyboardSelect::keyPressed_9()
{
	process(9);
}
