#ifndef TANO_TIMER_H_
#define TANO_TIMER_H_

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QTime>

class Timer : public QObject
{
Q_OBJECT
public:
	Timer(const QString &fname,
		  const QString &fchannel = 0,
		  const QString &fplaylist = 0,
		  const int &fnum = 0,
		  const QString &furl = 0);
	~Timer();

public slots:
	QString name() const {return _name;}
	void setName(const QString &sname);
	QString channel() const {return _channel;}
	void setChannel(const QString &schannel);
	QString playlist() const {return _playlist;}
	void setPlaylist(const QString &splaylist);
	QString url() const {return _url;}
	void setUrl(const QString &surl);
	int num() const {return _num;}
	void setNum(const int &snum);
	QDate date() const {return _date;}
	void setDate(const QDate &sdate);
	QTime startTime() const {return _startTime;}
	void setStartTime(const QTime &sstartTime);
	QTime endTime() const {return _endTime;}
	void setEndTime(const QTime &sendTime);
	bool isDisabled() const {return _disabled;}
	void setDisabled(const bool &disabled);

private:
	bool _disabled;
	QString _name;
	QString _channel;
	QString _playlist;
	QString _url;
	int _num;
	QDate _date;
	QTime _startTime;
	QTime _endTime;
};

#endif // TANO_TIMER_H_
