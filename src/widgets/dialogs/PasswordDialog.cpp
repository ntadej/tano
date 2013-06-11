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

#include "Config.h"
#include "core/Common.h"
#include "core/Log.h"
#include "core/network/NetworkRequest.h"
#include "core/settings/SettingsPassword.h"

#include "PasswordDialog.h"
#include "ui_PasswordDialog.h"

PasswordDialog::PasswordDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::PasswordDialog),
      _request(new NetworkRequest(this)),
      _password("")
{
    ui->setupUi(this);

    QScopedPointer<SettingsPassword> settings(new SettingsPassword(this));
    settings->setUid(Tano::uid());
    settings->writeSettings();

    ui->editUsername->setText(settings->username());
    if (!settings->password().isEmpty()) {
        ui->editPassword->setText(settings->password());
        ui->remember->setChecked(true);

        validatePassword();
    }

    connect(_request, SIGNAL(error(int, QNetworkReply *)), this, SLOT(validatePasswordResponse(int, QNetworkReply *)));
    connect(_request, SIGNAL(result(QByteArray, QNetworkReply *)), this, SLOT(validatePasswordResponse(QByteArray, QNetworkReply *)));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(validatePassword()));
}

PasswordDialog::~PasswordDialog()
{
    delete ui;
}

void PasswordDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void PasswordDialog::validatePassword(const bool &edit)
{
#if PASSWORD
    QUrl validator = QUrl(QString(PASSWORD_VALIDATE));
    _edit = edit;

    QNetworkRequest nr(validator);
    nr.setRawHeader("Authorization", "Basic " + QByteArray(QString("%1:%2").arg(ui->editUsername->text()).arg(ui->editPassword->text()).toLocal8Bit()).toBase64());
    _request->getRequest(nr);
#else
    Q_UNUSED(edit)
#endif
}

void PasswordDialog::validatePasswordResponse(const int &error,
                                              QNetworkReply *reply)
{
    Q_UNUSED(reply)

#if PASSWORD
    switch(error)
    {
    case QNetworkReply::AuthenticationRequiredError:
        ui->labelInfo->setText(tr("<b>Your username or password is incorrect.\nPlease, try again.</b>"));
        break;
    default:
        ui->labelInfo->setText(tr("<b>An unknown error has occured.\nPlease, try again.</b>"));
    }
#else
    Q_UNUSED(error)
#endif
}

void PasswordDialog::validatePasswordResponse(const QByteArray &response,
                                              QNetworkReply *reply)
{
    Q_UNUSED(reply)

#if PASSWORD
    if (QString(response).contains(QString(PASSWORD_KEY))) {
        QScopedPointer<SettingsPassword> settings(new SettingsPassword(this));
        settings->setUsername(ui->editUsername->text());
        settings->setPassword(ui->remember->isChecked() ? ui->editPassword->text() : "");
        settings->writeSettings();

        _password = ui->editPassword->text();

        Tano::Log::login();

        if (!_edit)
            accept();
    } else {
        validatePasswordResponse(99);
    }
#else
    Q_UNUSED(response)
#endif
}
