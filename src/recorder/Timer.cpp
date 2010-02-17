#include "Timer.h"

Timer::Timer(const QString &fname,
			 const QString &fchannel,
			 const QString &fplaylist,
			 const int &fnum,
			 const QString &furl)
	: _name(fname), _channel(fchannel), _playlist(fplaylist), _num(fnum), _url(furl)
{
	_date = QDate::currentDate();
	_startTime = QTime::currentTime();
	_endTime = QTime::currentTime().addSecs(3600);
	_disabled = false;
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

void Timer::setUrl(const QString &surl)
{
	_url = surl;
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

void Timer::setDisabled(const bool &disabled)
{
	_disabled = disabled;
}
