#ifndef VLCCONTROL_H_
#define VLCCONTROL_H_

#include <QTimer>
#include <QList>
#include <QMap>
#include <QAction>
#include <QActionGroup>

class VlcControl : public QObject {
Q_OBJECT
public:
	VlcControl();
	virtual ~VlcControl();

	void update();

private slots:
	void updateActions();
	void updateAudio();
	void updateSub();

signals:
	void vlcAction(QString, QList<QAction*>);

private:
	QTimer *timer;

	QList<QAction*> audioList;
	QMap<QString,int> audioMap;
	QActionGroup *audioGroup;

    QList<QAction*> subList;
	QMap<QString,int> subMap;
	QActionGroup *subGroup;
};

#endif /* VLCCONTROL_H_ */
