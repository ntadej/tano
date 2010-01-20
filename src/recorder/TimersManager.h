#ifndef TANO_TIMERSMANAGER_H_
#define TANO_TIMERSMANAGER_H_

#include <QtGui/QMainWindow>
#include <QSettings>
#include <QTreeWidgetItem>
#include <QMap>

#include "../channels/Channel.h"
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
    void newItem();
    void addItem();
    void edit(QTreeWidgetItem *item);
    void playlist(QTreeWidgetItem *item);
    void applyName(QString name);

private:
    Ui::TimersManager ui;

    QMap<QTreeWidgetItem*,Timer*> map;

    Timer *currentTimer;
    QTreeWidgetItem *currentItem;
    Channel *channel;
};

#endif // TANO_TIMERSMANAGER_H_
