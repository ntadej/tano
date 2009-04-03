#ifndef TANO_H
#define TANO_H

#include <QtGui/QMainWindow>
#include <QCloseEvent>
#include <QTime>
#include <QSettings>

#include "ui_Tano.h"
#include "Updates.h"
#include "xml/tanohandler.h"
#include "channels/Channel.h"
#include "channels/EditPlaylist.h"
#include "ui/EditSettings.h"
#include "ui/TrayIcon.h"
#include "ui/TanoOsd.h"
#include "control/ChannelSelect.h"
#include "control/Shortcuts.h"
#include "epg/Epg.h"
#include "epg/EpgBrowser.h"
#include "recorder/Recorder.h"

class Tano : public QMainWindow
{
    Q_OBJECT

public:
    Tano(QWidget *parent = 0);
    ~Tano();

protected:
	void closeEvent(QCloseEvent *event);

private slots:
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
    void showSiolEpg();

    void showEpg(QStringList epgValue);

    void processUpdates(QString updates);

    void tooltip(QString channelNow = "stop");

    void actionChannelShow();
    void actionToolbarShow();

    void rightMenu(QPoint pos);
    void menuOpen();
    void top();
    void lite();

    void recorder();
    void help();

    void time(qint64 t);
    void totalTime(qint64 t);

private:
    Ui::Tano ui;
    Qt::WindowFlags flags;

    Updates *update;
    Recorder *record;
    TanoOsd *osd;
    QSettings *settings;

    TanoHandler *handler;
    Channel *channel;
	QString fileName;
	QString defaultP;

	TrayIcon *trayIcon;
	Epg *epg;
	EpgBrowser *browser;
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

    void createConnections();
    void createMenus();
    void createShortcuts();
    void createSettings();
    void createOsd();
};

#endif // TANO_H
