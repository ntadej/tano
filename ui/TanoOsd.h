#ifndef TANOOSD_H
#define TANOOSD_H

#include <QtGui/QWidget>
#include <QTimer>

#include "ui_TanoOsd.h"

class TanoOsd : public QWidget
{
    Q_OBJECT

public:
    TanoOsd(QWidget *parent = 0);
    ~TanoOsd();

public slots:
	void showOsd();
	void hideOsd();

private:
    Ui::TanoOsdClass ui;

    QTimer *timer;
};

#endif // TANOOSD_H
