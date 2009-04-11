#ifndef TANO_CHANNELEPG_H_
#define CHANNELEPG_H_
#include <QString>

class ChannelEpg {
public:
	ChannelEpg(QString wtime, QString wurl, QString wtitle);
	virtual ~ChannelEpg();

	QString time();
	QString url();
	QString title();
private:
	QString _time;
	QString _url;
	QString _title;
};

#endif // TANO_CHANNELEPG_H_
