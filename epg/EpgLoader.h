#ifndef TANO_EPGLOADER_H_
#define TANO_EPGLOADER_H_

#include <QHttp>
#include <QString>
#include <QStringList>
#include <QTextCodec>
#include <QTextEdit>

class EpgLoader : public QHttp {
Q_OBJECT
public:
	EpgLoader();
	virtual ~EpgLoader();

	void getEpg(bool f, QString epgP);
	void stop();

signals:
	void epgDone(QStringList);

private slots:
	void epg();
	void epgPrint();
	void epgShow();
	void epgInit();
	void epgInitDone();

private:
	bool init;
	bool full;

	QString epgGet;
	QString epgValue;
	QString epgFull;
	QString epgFlag;
	QStringList epgList;
	QStringList epgListShow;

	QTextCodec *codec;
	QTextEdit *edit;
};

#endif /* TANO_EPGLOADER_H_ */
