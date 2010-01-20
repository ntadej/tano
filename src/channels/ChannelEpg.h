#ifndef TANO_CHANNELEPG_H_
#define TANO_CHANNELEPG_H_

#include <QString>

class ChannelEpg {
public:
	ChannelEpg(QString wtime, QString wurl, QString wtitle);
	virtual ~ChannelEpg();

	QString time() const {return _time;};
	QString url() const {return _url;};
	QString title() const {return _title;};

private:
	QString _time;
	QString _url;
	QString _title;
};

#endif // TANO_CHANNELEPG_H_
