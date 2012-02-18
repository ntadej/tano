/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_UPDATEDIALOG_H_
#define TANO_UPDATEDIALOG_H_

#include <QtGui/QAbstractButton>
#include <QtGui/QDialog>

#include "core/UpdateManager.h"

namespace Ui {
    class UpdateDialog;
}

class UpdateDialog : public QDialog
{
Q_OBJECT
public:
    UpdateDialog(QWidget *parent = 0);
    ~UpdateDialog();

public slots:
    void check();
    void checkSilent();

protected:
    void changeEvent(QEvent *e);

signals:
    void newUpdate();

private slots:
    void action(QAbstractButton *button);
    void processUpdate(const QStringList &update,
                       const UpdateInfo &info);

private:
    Ui::UpdateDialog *ui;

    QString generateUrl(const QString &version);

    UpdateManager *_update;

    bool _silent;
};

#endif // TANO_UPDATEDIALOG_H_
