#ifndef VLCCONTROL_H_
#define VLCCONTROL_H_

#include <QTimer>
#include <QMenu>
#include <QList>
#include <QMap>
#include <QAction>
#include <QActionGroup>

class VlcControl : public QObject {
Q_OBJECT
public:
	VlcControl();
	virtual ~VlcControl();

private slots:
	void updateActions();
	void updateAudio();
	void updateSub();

signals:
	void vlcAction(QString, QMenu*);

private:
	QTimer *timer;

	QList<QAction*> audioList;
	QMap<int,QString> audioMap;
	QMenu *audioMenu;
	QActionGroup *audioGroup;

    QList<QAction*> subList;
	QMap<int,QString> subMap;
	QMenu *subMenu;
	QActionGroup *subGroup;
};

#endif /* VLCCONTROL_H_ */
