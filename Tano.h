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

    void actionShow(bool status);

    void rightMenu(QPoint pos);
    void menuOpen();

private:
    Ui::Tano ui;

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

	QMenu *right;
	QMenu *open;
	QActionGroup *ratioGroup;
	QActionGroup *cropGroup;

    void createActions();
    void createMenus();
};

#endif // TANO_H
