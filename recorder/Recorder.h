#ifndef RECORDER_H
#define RECORDER_H

#include <QtGui/QMainWindow>
#include <QMenu>
#include <QProcess>
#include <QTime>
#include <QTimer>

#include "../channels/Channel.h"
#include "../ui/TrayRecorder.h"
#include "../xml/tanohandler.h"

#include "ui_Recorder.h"

class Recorder : public QMainWindow
{
    Q_OBJECT

public:
    Recorder(QWidget *parent = 0);
    ~Recorder();

protected:
	void closeEvent(QCloseEvent *event);

public slots:
	void showRecorder();
	void closeRecorder();

private slots:
	void about();
	void help();

	void record(bool status);

	void sec();

	void openPlaylist();
	void playlist(QTreeWidgetItem* clickedChannel);
	void fileBrowse();

private:
    Ui::RecorderClass ui;

    bool recording;
    bool start;
    Channel *channel;

	TrayRecorder *trayIcon;
	QMenu *tray;

	TanoHandler *handler;
	QString fileName;

	QProcess *frip;
	QString fripPath;
	QString slash;

	QTimer *timer;
	QTime time;
};

#endif // RECORDER_H
