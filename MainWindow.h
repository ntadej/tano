#ifndef TANO_MAINWINDOW_H
#define TANO_MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QCloseEvent>
#include <QTime>
#include <QSettings>

#include "ui_MainWindow.h"
#include "Updates.h"
#include "xml/tanohandler.h"
#include "channels/Channel.h"
#include "channels/EditPlaylist.h"
#include "ui/EditSettings.h"
#include "ui/TrayIcon.h"
#include "ui/Osd.h"
#include "control/ChannelSelect.h"
#include "control/Shortcuts.h"
#include "epg/Epg.h"
#include "epg/EpgBrowser.h"
#include "epg/EpgShow.h"
#include "recorder/Recorder.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void exit();

    void aboutTano();
    void playlist(QTreeWidgetItem* clickedChannel);
    void key(int clickedChannel);

    void play();
    void stop();

    void openPlaylist(bool start = false);
    void openFile();
    void openUrl();
    void showSettings();
    void showBrowser();

    void showEpg(int id, QStringList epgValue, QString date);

    void processUpdates(QString updates);

    void tooltip(QString channelNow = "stop");

    void rightMenu(QPoint pos);
    void menuOpen();
    void top();
    void lite();

    void recorder();
    void help();

    void time(qint64 t);
    void totalTime(qint64 t);
    void volumeControl(bool type);

private:
    Ui::MainWindow ui;
    Qt::WindowFlags flags;

    Updates *update;
    Recorder *record;
    Osd *osd;
    QSettings *settings;

    TanoHandler *handler;
    Channel *channel;
	QString fileName;
	QString defaultP;

	TrayIcon *trayIcon;
	Epg *epg;
	EpgBrowser *browser;
	EpgShow *epgShow;
	EditPlaylist *editor;

	Shortcuts *shortcuts;
	ChannelSelect *select;

	QMenu *right;
	QMenu *open;
	QMenu *tray;
	QActionGroup *ratioGroup;
	QActionGroup *cropGroup;
	QList<QAction*> actions;

	QTime timeNow;

	bool isLite;
	bool osdEnabled;
	bool sessionEnabled;
	bool hasPlaylist;

    void createConnections();
    void createMenus();
    void createShortcuts();
    void createSettings();
    void createSession();
    void createOsd();
};

#endif // TANO_MAINWINDOW_H
