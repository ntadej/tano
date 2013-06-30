/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_PASSWORDDIALOG_H_
#define TANO_PASSWORDDIALOG_H_

#include <QtNetwork/QNetworkReply>

#if QT_VERSION >= 0x050000
    #include <QtWidgets/QDialog>
#else
    #include <QtGui/QDialog>
#endif

#include "WidgetsSharedExport.h"

class NetworkRequest;

namespace Ui {
    class PasswordDialog;
}

class TANO_WIDGETS_EXPORT PasswordDialog : public QDialog
{
Q_OBJECT

public:
    explicit PasswordDialog(QWidget *parent = 0);
    ~PasswordDialog();

    inline const QString password() { return _password; }

protected:
    void changeEvent(QEvent *e);

private slots:
    void validatePassword(bool edit = false);
    void validatePasswordResponse(int error,
                                  QNetworkReply *reply = 0);
    void validatePasswordResponse(const QByteArray &response,
                                  QNetworkReply *reply = 0);
    
private:
    Ui::PasswordDialog *ui;

    NetworkRequest *_request;
    QString _password;
    bool _edit;
};

#endif // TANO_PASSWORDDIALOG_H_
