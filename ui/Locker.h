#ifndef LOCKER_H
#define LOCKER_H

#include <QtGui/QDialog>
#include <QSettings>
#include <QCryptographicHash>

#include "ui_Locker.h"

class Locker : public QDialog
{
    Q_OBJECT

public:
    Locker(QWidget *parent = 0);
    ~Locker();

private slots:
	void action(QAbstractButton *button);
	void checkPassword();

private:
    Ui::Locker ui;

    QSettings *settings;
};

#endif // LOCKER_H
