#ifndef EPGTODAY_H
#define EPGTODAY_H

#include <QtGui/QDialog>
#include "ui_EpgToday.h"

class EpgToday : public QDialog
{
    Q_OBJECT

public:
    EpgToday(QWidget *parent = 0);
    ~EpgToday();

    void setEpg(QString epg);

public slots:
    void showEpg();

private:
    Ui::EpgTodayClass ui;

    QStringList epgList;
    QTableWidgetItem *newItem;
};

#endif // EPGTODAY_H
