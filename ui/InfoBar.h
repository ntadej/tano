#ifndef INFOBAR_H
#define INFOBAR_H

#include <QtGui/QWidget>

#include "ui_InfoBar.h"

class InfoBar : public QWidget
{
    Q_OBJECT

public:
    InfoBar(QWidget *parent = 0);
    ~InfoBar();

public slots:
	void clear();
	void setInfo(QString channel, QString language);
	void setEpg(QString now, QString next);

signals:
	void open(QString);
	void refresh();

private:
    Ui::InfoBar ui;

};

#endif // INFOBAR_H
