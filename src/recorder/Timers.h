#ifndef TIMERS_H
#define TIMERS_H

#include <QtGui/QMainWindow>
#include <QSettings>

#include "ui_Timers.h"

class Timers : public QMainWindow
{
    Q_OBJECT

public:
    Timers(QWidget *parent = 0);
    ~Timers();

private:
    Ui::Timers ui;
};

#endif // TIMERS_H
