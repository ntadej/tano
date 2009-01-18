#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

#ifndef TRAYICON_H_
#define TRAYICON_H_

class TrayIcon : public QSystemTrayIcon {
Q_OBJECT
public:
	TrayIcon();
	virtual ~TrayIcon();

	void message(QString type);

public slots:
	void changeToolTip(QString text = "stop");

signals:
	void restoreClick();
	void quitClick();

private slots:
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
	void eRestore();
	void eQuit();

private:
	QMenu *trayIconMenu;
	QAction *actionRestore;
	QAction *actionQuit;
};

#endif /* TRAYICON_H_ */
