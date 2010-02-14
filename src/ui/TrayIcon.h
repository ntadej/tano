#ifndef TANO_TRAYICON_H_
#define TANO_TRAYICON_H_

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

class TrayIcon : public QSystemTrayIcon {
Q_OBJECT
public:
	TrayIcon(QMenu *menu);
	virtual ~TrayIcon();

public slots:
	void message(QStringList arg);
	void changeToolTip(const QString &text = "stop", const QString &type = "main");

signals:
	void restoreClick();

private slots:
	void iconActivated(QSystemTrayIcon::ActivationReason reason);

private:
	QString _currentlyPlaying;
	QString _currentlyRecording;
};

#endif // TANO_TRAYICON_H_
