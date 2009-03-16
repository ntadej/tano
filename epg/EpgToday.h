#ifndef EPGTODAY_H
#define EPGTODAY_H

#include <QTableWidget>
#include <QMap>
#include "../channels/ChannelEpg.h"

class EpgToday : public QTableWidget
{
    Q_OBJECT

public:
    EpgToday(QWidget *parent = 0);
    ~EpgToday();

    void epgClear();

public slots:
    void setEpg(QStringList epg);

signals:
	void urlClicked(QString);

private slots:
	void processEpg();
	void epgClicked(QTableWidgetItem *item);

private:
    QStringList epgList;
    ChannelEpg *newEpg;
    QTableWidgetItem *newItem;
    QMap<QTableWidgetItem*, ChannelEpg*> map;
};

#endif // EPGTODAY_H
