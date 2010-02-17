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
	for(int i=0; i<_timersList.size(); i++) {
		if(_timersList[i]->startTime() >= QTime::currentTime()) {
			emit startTimer(_timersList[i]);
		} else if(_timersList[i]->endTime() <= QTime::currentTime()) {
			emit startTimer(_timersList[i]);
		} else if(_timersList[i]->endTime() >= QTime::currentTime()) {
			emit stopTimer(_timersList[i]);
		}
	}
}

void Time::addTimer(const Timer *t)
{
	if(!_timersList.contains(t))
		_timersList.append(t);
}

void Time::removeTimer(const Timer *t)
{
	if(_timersList.contains(t))
		_timersList.removeOne(t);
}
