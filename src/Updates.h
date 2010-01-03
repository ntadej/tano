#ifndef UPDATES_H_
#define UPDATES_H_

#include <QHttp>
#include <QString>
#include <QTextCodec>

class Updates : public QHttp {
Q_OBJECT
public:
	Updates();
	virtual ~Updates();

public slots:
	void getUpdates();

signals:
	void updatesDone(QString);

private slots:
	void updatesPrint();

private:
	QTextCodec *codec;
};

#endif
