#ifndef TANO_H
#define TANO_H

#include <QtGui/QMainWindow>
#include <QCloseEvent>

#include "ui_Tano.h"
#include "Updates.h"
#include "xml/tanohandler.h"
#include "channels/Channel.h"
#include "channels/EditPlaylist.h"
#include "settings/EditSettings.h"
#include "ui/TrayIcon.h"
#include "ui/KeyboardSelect.h"
#include "epg/Epg.h"
#include "epg/EpgBrowser.h"

class Tano : public QMainWindow
{
    Q_OBJECT

public:
    Tano(QWidget *parent = 0, QString defaultPlaylist = "siol.xml");
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
    void settings();
    void showBrowser();
    void showSiolEpg();

    void showEpg(QString epgValue, bool full);

    void processUpdates(QString updates);

    void tooltip(QString channelNow = "stop");

    void actionChannelShow();
    void actionToolbarShow();

    void rightMenu(QPoint pos);
    void menuOpen();
    void top();
    void lite();

private:
    Ui::Tano ui;
    Qt::WindowFlags flags;

    QString version;
    QString build;

    Updates *update;

    TanoHandler *handler;
    Channel *channel;
	QString fileName;
	QString defaultP;
	TrayIcon *trayIcon;
	Epg *epg;
	EpgBrowser *browser;
	EditPlaylist *editor;
	KeyboardSelect *keyboard;

	QMenu *right;
	QMenu *open;
	QMenu *tray;
	QActionGroup *ratioGroup;
	QActionGroup *cropGroup;

	bool isLite;

    void createActions();
    void createMenus();
};

#endif // TANO_H
