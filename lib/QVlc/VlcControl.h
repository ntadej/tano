#ifndef QVLC_VLCCONTROL_H_
#define QVLC_VLCCONTROL_H_

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QTimer>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>

class VlcControl : public QObject {
Q_OBJECT
public:
	VlcControl();
	virtual ~VlcControl();

private slots:
	void updateActionsAudio();
	void updateActionsVideo();
	void updateAudio();
	void updateSub();

	void checkPlayingState();

signals:
	void vlcAction(QString, QList<QAction*>);
	void stateChanged(int);

private:
	QTimer *timer;
	QTimer *check;

	QList<QAction*> audioList;
	QMap<QString,int> audioMap;
	QActionGroup *audioGroup;

    QList<QAction*> subList;
	QMap<QString,int> subMap;
	QActionGroup *subGroup;
};

#endif // QVLC_VLCCONTROL_H_
