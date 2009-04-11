#ifndef TANO_CHANNELSELECT_H_
#define TANO_CHANNELSELECT_H_

#include <QShortcut>
#include <QLCDNumber>
#include <QTimer>

class ChannelSelect : public QObject {
Q_OBJECT
public:
	ChannelSelect(QWidget *parent, QLCDNumber *number, QList<int> l);
	virtual ~ChannelSelect();

public slots:
	void back();
	void next();
	void channel(bool direction);

private slots:
    void keyPressed_0();
    void keyPressed_1();
    void keyPressed_2();
    void keyPressed_3();
    void keyPressed_4();
    void keyPressed_5();
    void keyPressed_6();
    void keyPressed_7();
    void keyPressed_8();
    void keyPressed_9();

    void display();

signals:
	void channelSelect(int);
	void error(QString, int);

private:
	void process(int key);

	QShortcut *key_0;
	QShortcut *key_1;
	QShortcut *key_2;
	QShortcut *key_3;
	QShortcut *key_4;
	QShortcut *key_5;
	QShortcut *key_6;
	QShortcut *key_7;
	QShortcut *key_8;
	QShortcut *key_9;

	QList<int> lim;
	QLCDNumber *lcd;
	QTimer *timer;
	int num;
	int number1;
	int number2;
	int number3;
	int full;
	int old;
};

#endif // TANO_CHANNELSELECT_H_
