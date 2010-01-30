#ifndef TANO_TIME_H_
#define TANO_TIME_H_

#include <QTimer>
#include <QList>

#include "../recorder/Timer.h"

class Time : public QObject {
Q_OBJECT
public:
	Time();
	virtual ~Time();

	void addTimer(Timer *t);
	void removeTimer(Timer *t);

signals:
	void startTimer(Timer*);

private slots:
	void check();

private:
	QTimer *time;
	QList<Timer*> timersList;
};

#endif // TANO_TIME_H_
