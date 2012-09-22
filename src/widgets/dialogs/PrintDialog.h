/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef TANO_PRINTDIALOG_H_
#define TANO_PRINTDIALOG_H_

#if defined(Qt5)
    #include <QtWidgets/QDialog>
#elif defined(Qt4)
    #include <QtGui/QDialog>
#endif

#include "core/Print.h"

namespace Ui {
    class PrintDialog;
}

class QAbstractButton;

class PrintDialog : public QDialog
{
Q_OBJECT
public:
    PrintDialog(const QString &name,
                PlaylistModel *model,
                QWidget *parent = 0);
    ~PrintDialog();

protected:
    void changeEvent(QEvent *e);

private slots:
    void action(QAbstractButton *button);

private:
    void print();

    Ui::PrintDialog *ui;

    Print *_print;

    QString _name;
    PlaylistModel *_model;
};

#endif // TANO_PRINTDIALOG_H_
