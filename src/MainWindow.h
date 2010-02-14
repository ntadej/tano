#ifndef TANO_MAINWINDOW_H_
#define TANO_MAINWINDOW_H_

#include <QtGui/QMainWindow>
#include <QCloseEvent>
#include <QSettings>

#include <QVlc/VlcInstance.h>
#include <QVlc/VlcControl.h>

#include <ui_MainWindow.h>

#include "Updates.h"
#include "channels/Channel.h"
#include "channels/EditPlaylist.h"
#include "control/ChannelSelect.h"
#include "control/Shortcuts.h"
#include "control/Time.h"
#include "epg/Epg.h"
#include "epg/EpgShow.h"
#include "recorder/TimersManager.h"
#include "ui/Osd.h"
#include "ui/TrayIcon.h"

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

	void play(QString itemFile = 0, QString itemType = 0);
	void stop();

	void playingState(int status);

	void openPlaylist(bool start = false);
	void openFile();
	void openUrl();
	void showSettings();

	void showEpg(int id, QStringList epgValue, QString date);

	void tooltip(QString channelNow = "stop");

	void showRightMenu(QPoint pos);
	void menuOpen();
	void top();
	void lite();
	void tray();

	void recordNow();
	void recorder(bool enabled);
	void processMenu(QString type, QList<QAction*> list);

signals:
	void setVolume(int);

private:
	//Initialising functions
	void createBackend();
	void createCommon();
	void createConnections();
	void createGui();
	void createMenus();
	void createOsd();
	void createRecorder();
	void createSettings();
	void createSession();
	void createShortcuts();

	//Settings
	QSettings *settings;
	QString _defaultPlaylist;
	QString _defaultSubtitleLanguage;
	bool _hasPlaylist;
	bool _isLite;
	bool _osdEnabled;
	bool _recorderEnabled;
	int _sessionChannel;
	bool _sessionEnabled;
	int _sessionVolume;
	bool _updatesOnStart;
	bool _videoSettings;
	QString _wheelType;

	//Main
	ChannelSelect *select;
	Shortcuts *shortcuts;
	Time *time;
	Updates *update;

	//Backend
	VlcInstance *backend;
	VlcControl *controller;

	//GUI
	Ui::MainWindow ui;
	Qt::WindowFlags flags;
	Osd *osd;

	//Playback and channels
	Channel *channel;
	Epg *epg;
	EpgShow *epgShow;
	EditPlaylist *editor;
	QString _fileName;
	QString _playlistName;

	//Menus and actions
	TrayIcon *trayIcon;
	QMenu *rightMenu;
	QMenu *openMenu;
	QActionGroup *ratioGroup;
	QActionGroup *cropGroup;
	QActionGroup *filterGroup;
	QList<QAction*> actions;

	//Recorder
	TimersManager *timers;
};

#endif // TANO_MAINWINDOW_H_
