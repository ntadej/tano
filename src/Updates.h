#ifndef TANO_UPDATES_H_
#define TANO_UPDATES_H_

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
	void updatesDone(QStringList);

private slots:
	void processUpdates(QString v);
	void updatesPrint();

private:
	QTextCodec *codec;
};

#endif // TANO_UPDATES_H_
