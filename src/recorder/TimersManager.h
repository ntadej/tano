#ifndef TANO_TIMERSMANAGER_H_
#define TANO_TIMERSMANAGER_H_

#include <QMainWindow>
#include <QSettings>
#include <QTreeWidgetItem>
#include <QMap>

#include "../channels/Channel.h"
#include "../xml/TimersHandler.h"
#include "Timer.h"

#include <ui_TimersManager.h>

class TimersManager : public QMainWindow
{
    Q_OBJECT

public:
    TimersManager(QWidget *parent = 0);
    ~TimersManager();

    void openPlaylist(QString file);

private slots:
	void action(QAbstractButton*);

    void newItem();
    void addItem();
    void edit(QTreeWidgetItem *item);
    void playlist(QTreeWidgetItem *item);
    void applyName(QString name);

    void read(QString file = 0);
    void write();

private:
    Ui::TimersManager ui;

    Timer *currentTimer;
    QTreeWidgetItem *currentItem;
    Channel *channel;

    TimersHandler *handler;
};

#endif // TANO_TIMERSMANAGER_H_
