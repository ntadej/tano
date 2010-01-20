#ifndef TANO_RECORDER_H_
#define TANO_RECORDER_H_

#include <QtGui/QMainWindow>
#include <QMenu>
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

public slots:
	void stop();
	void recordNow(int nmb, QString url, QString name);
	void openPlaylist(QString file);

private slots:
	void record(bool status);

	void sec();

	void playlist(QTreeWidgetItem* clickedChannel);
	void fileBrowse();

private:
    Ui::Recorder ui;

    bool recording;
    Channel *channel;

	TrayIcon *trayIcon;
	QMenu *tray;

	QProcess *frip;
	QString fripPath;
	QString slash;

	QTimer *timer;
	QTime time;

	QSettings *settings;
};

#endif // TANO_RECORDER_H_
