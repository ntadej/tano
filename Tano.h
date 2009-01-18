#ifndef TANO_H
#define TANO_H

#include <QtGui/QMainWindow>
#include <QCloseEvent>
#include <QTimer>

#include "ui_Tano.h"
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

    void openPlaylist(bool start = false);
    void openFile();
    void editPlaylist();
    void settings();
    void showBrowser();
    void browser(QString link);

    void marque();
    void updateMarque(int value);

    void showEpg(QString epgValue, bool full);

    void vlc();
    void help();

    void tooltip(QString channelNow = "stop");

private:
    Ui::Tano ui;

    QString version;
    QString date;

    TanoHandler *handler;
    Channel *channel;
	QString fileName;
	QString defaultP;
	TrayIcon *trayIcon;
	Epg *epg;
	QTimer *mtimer;
	int scroll;
	int timer;
	QString vlcStatus;

    void createActions();
};

#endif // TANO_H
