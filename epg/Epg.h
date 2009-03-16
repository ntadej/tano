#ifndef EPG_H_
#define EPG_H_

#include <QHttp>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QTextCodec>
#include <QTextEdit>

class Epg : public QHttp {
Q_OBJECT
public:
	Epg();
	virtual ~Epg();

	void getEpg(QString epgP);

public slots:
	void refresh();
	void stop();

signals:
	void epgDone(QString);
	void epgDoneFull(QStringList);

private slots:
	void epg();
	void epgPrint();

private:
	void epgNow();

	QString epgValue;
	QString epgFull;
	QString epgChannel;
	QStringList epgList;

	QTimer *timer;
	QTextCodec *codec;
	QTextEdit *edit;
};

#endif /* EPG_H_ */
