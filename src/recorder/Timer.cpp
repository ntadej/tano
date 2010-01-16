#include "Timer.h"

Timer::Timer(QString fname, QString fchannel, QString fplaylist, int fnum)
	: _name(fname), _channel(fchannel), _playlist(fplaylist), _num(fnum)
{
	_date = QDate::currentDate();
	_startTime = QTime(0,0,0,0);
	_endTime = QTime(0,0,0,0);
}

Timer::~Timer() {

}

void Timer::setName(const QString &sname)
{
	_name = sname;
}

void Timer::setChannel(const QString &schannel)
{
	_channel = schannel;
}

void Timer::setPlaylist(const QString &splaylist)
{
	_playlist = splaylist;
}

void Timer::setNum(const int &snum)
{
	_num = snum;
}

void Timer::setDate(const QDate &sdate)
{
	_date = sdate;
}

void Timer::setStartTime(const QTime &sstartTime)
{
	_startTime = sstartTime;
}

void Timer::setEndTime(const QTime &sendTime)
{
	_endTime = sendTime;
}
