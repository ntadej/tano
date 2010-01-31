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
	for(int i=0; i<timersList.size(); i++) {
		if(timersList[i]->startTime() >= QTime::currentTime()) {
			emit startTimer(timersList[i]);
		} else if(timersList[i]->endTime() >= QTime::currentTime()) {
			emit startTimer(timersList[i]);
		}
	}
}

void Time::addTimer(const Timer *t)
{
	if(!timersList.contains(t))
		timersList.append(t);
}

void Time::removeTimer(const Timer *t)
{
	if(timersList.contains(t))
		timersList.removeOne(t);
}
