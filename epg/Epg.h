#ifndef TANO_EPG_H_
#define TANO_EPG_H_

#include <QString>
#include <QStringList>
#include <QTimer>

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
	void epgDone(QStringList);
	void epgDoneFull(QStringList);

private slots:
	void epgNow();
	void epgSet(QStringList list);

private:
	bool init;

	EpgLoader *loader;

	QString epgFull;
	QString epgChannel;
	QStringList epgList;
	QStringList epgListNow;

	QTimer *timer;
};

#endif /* EPG_H_ */
