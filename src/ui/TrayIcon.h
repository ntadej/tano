#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

#ifndef TRAYICON_H_
#define TRAYICON_H_

class TrayIcon : public QSystemTrayIcon {
Q_OBJECT
public:
	TrayIcon(QMenu *menu);
	virtual ~TrayIcon();

	void message(QString type);

public slots:
	void changeToolTip(QString text = "stop");

signals:
	void restoreClick();

private slots:
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
	void eRestore();
};

#endif /* TRAYICON_H_ */
