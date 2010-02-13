#ifndef TANO_RECORDER_H_
#define TANO_RECORDER_H_

#include <QtGui/QMainWindow>
#include <QProcess>
#include <QTime>
#include <QTimer>
#include <QSettings>

#include "../channels/Channel.h"
#include "../ui/TrayIcon.h"
#include "../xml/M3UHandler.h"

#include <ui_Recorder.h>

class Recorder : public QWidget
{
	Q_OBJECT

public:
	Recorder(QWidget *parent = 0);
	~Recorder();

	bool isRecording();
	void setGlobals(TrayIcon *icon, QAction *action);

public slots:
	void stop();
	void recordNow(const int nmb, const QString &url, const QString &name);
	void openPlaylist(const QString &file);

private slots:
	void record(const bool status);

	void sec();

	void playlist(QTreeWidgetItem* clickedChannel);
	void fileBrowse();

private:
	Ui::Recorder ui;

	bool recording;
	Channel *channel;

	TrayIcon *trayIcon;

	QProcess *frip;
	QString fripPath;
	QString slash;

	QTimer *timer;
	QTime time;

	QSettings *settings;

	QAction *actionRecord;
};

#endif // TANO_RECORDER_H_
