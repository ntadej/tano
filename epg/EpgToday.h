#ifndef EPGTODAY_H
#define EPGTODAY_H

#include <QtGui/QWidget>
#include <QMap>
#include "../channels/ChannelEpg.h"
#include "ui_EpgToday.h"

class EpgToday : public QWidget
{
    Q_OBJECT

public:
    EpgToday(QWidget *parent = 0);
    ~EpgToday();

    void setEpg(QString epg);

signals:
	void urlClicked(QString);

public slots:
    void showEpg();

private slots:
	void processEpg();
	void epgClicked(QTableWidgetItem *item);

private:
    Ui::EpgTodayClass ui;

    QStringList epgList;
    ChannelEpg *newEpg;
    QTableWidgetItem *newItem;
    QMap<QTableWidgetItem*, ChannelEpg*> map;
};

#endif // EPGTODAY_H
