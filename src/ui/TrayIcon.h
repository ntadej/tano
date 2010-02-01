#ifndef TANO_TRAYICON_H_
#define TANO_TRAYICON_H_

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

class TrayIcon : public QSystemTrayIcon {
Q_OBJECT
public:
	TrayIcon(QMenu *menu, bool recorder = false);
	virtual ~TrayIcon();

public slots:
	void message(QStringList arg);
	void changeToolTip(QString text = "stop");

signals:
	void restoreClick();

private slots:
	void iconActivated(QSystemTrayIcon::ActivationReason reason);

private:
	bool _recorder;
};

#endif // TANO_TRAYICON_H_
