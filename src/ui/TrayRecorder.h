#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

#ifndef TRAYRECORDER_H_
#define TRAYRECORDER_H_

class TrayRecorder : public QSystemTrayIcon {
Q_OBJECT
public:
	TrayRecorder(QMenu *menu);
	virtual ~TrayRecorder();

	void message(QStringList arg);

public slots:
	void changeToolTip(QString text = "stop");

signals:
	void restoreClick();

private slots:
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
	void eRestore();
};

#endif /* TRAYRECORDER_H_ */
