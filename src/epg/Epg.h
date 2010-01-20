#ifndef TANO_EPG_H_
#define TANO_EPG_H_

#include <QString>
#include <QStringList>
#include <QTimer>
#include <QDateTime>

#include "EpgLoader.h"

class Epg : public QObject {
Q_OBJECT
public:
	Epg();
	virtual ~Epg();

	void getEpg(QString epgP);

public slots:
	void refresh();
	void reload();
	void stop();

signals:
	void epgDone(int, QStringList, QString);

private slots:
	void epgNow();
	void epgSet(QStringList list);
	void epgSetFull(QStringList list);

private:
	bool init;
	int step;

	EpgLoader *loader;

	QString epgFull;
	QString epgChannel;
	QStringList epgList;
	QStringList epgListNow;

	QTimer *timer;
	QDateTime date;
};

#endif // TANO_EPG_H_
