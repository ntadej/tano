#ifndef RECORDER_H
#define RECORDER_H

#include <QtGui/QMainWindow>
#include <QMenu>

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

	void openPlaylist();
	void playlist(QTreeWidgetItem* clickedChannel);

private:
    Ui::RecorderClass ui;

    bool start;
    Channel *channel;

	TrayRecorder *trayIcon;
	QMenu *tray;

	TanoHandler *handler;
	QString fileName;
};

#endif // RECORDER_H
