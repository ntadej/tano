#ifndef TANO_EPGLIST_H_
#define TANO_EPGLIST_H_

#include <QTableWidget>
#include <QMap>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>

#include "../channels/ChannelEpg.h"

class EpgList : public QTableWidget
{
    Q_OBJECT

public:
    EpgList(QWidget *parent = 0);
    ~EpgList();

    void epgClear();

public slots:
    void setEpg(QStringList epg);

signals:
	void urlClicked(QString);

protected:
	void mouseReleaseEvent(QMouseEvent *event);

private slots:
	void processEpg();
	void epgClicked(QTableWidgetItem *item = 0);

private:
    QStringList epgList;
    ChannelEpg *newEpg;
    QTableWidgetItem *newItem;
    QMap<QTableWidgetItem*, ChannelEpg*> map;

    QMenu *rightMenu;
    QAction *info;
    QAction *record;
};

#endif // TANO_EPGLIST_H_
