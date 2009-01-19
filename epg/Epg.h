#ifndef EPG_H_
#define EPG_H_

#include <QHttp>
#include <QString>
#include <QTimer>
#include <QTextCodec>

class Epg : public QHttp {
Q_OBJECT
public:
	Epg();
	virtual ~Epg();

	void getEpg(QString epgP);

public slots:
	void refresh();

signals:
	void epgDone(QString, bool);

private slots:
	void epg();
	void epgToday();
	void epgPrint();
	void epgPrintToday();

private:
	int step;
	QString epgValue;
	QString epgSource;
	QString epgUrl;
	QString epgFull;
	QTimer *timer;
	QTextCodec *codec;
};

#endif /* EPG_H_ */
