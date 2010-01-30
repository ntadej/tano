#include <QDebug>

#include "Time.h"

Time::Time() {

	time = new QTimer();
	connect(time, SIGNAL(timeout()), this, SLOT(check()));
	time->start(6000);
}

Time::~Time() {
	delete time;
}

void Time::check()
{
	qDebug() << "Time check" << timersList.size();
}

void Time::addTimer(Timer *t)
{
	if(!timersList.contains(t))
		timersList.append(t);
}

void Time::removeTimer(Timer *t)
{
	if(timersList.contains(t))
		timersList.removeOne(t);
}
