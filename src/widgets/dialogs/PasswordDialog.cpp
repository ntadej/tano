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

#include <QtCore/QCryptographicHash>
#include <QtNetwork/QNetworkReply>

#include "core/plugins/Plugins.h"
#include "core/settings/SettingsPassword.h"

#include "PasswordDialog.h"
#include "ui_PasswordDialog.h"

PasswordDialog::PasswordDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::PasswordDialog),
      _password("")
{
    ui->setupUi(this);

    ui->labelIcon->setPixmap(QIcon(":/logo/128x128/logo.png").pixmap(128));

    QScopedPointer<SettingsPassword> settings(new SettingsPassword(this));
    ui->editUsername->setText(settings->username());
    if (!settings->password().isEmpty()) {
        _password = settings->password();
        ui->remember->setChecked(true);

        validatePassword();
    }

    if (globalNetwork) {
        connect(globalNetwork, SIGNAL(passwordError(int)), this, SLOT(validatePasswordError(int)));
        connect(globalNetwork, SIGNAL(passwordOk(QString)), this, SLOT(validatePasswordOk(QString)));
    }

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

void PasswordDialog::validatePassword()
{
    ui->labelInfo->setText(tr("<b>Logging in ...</b>"));

    QString password = ui->editPassword->text().isEmpty() ? _password : QString(QCryptographicHash::hash(ui->editPassword->text().toLocal8Bit(), QCryptographicHash::Md5).toHex());

    if (globalNetwork) globalNetwork->authentication(ui->editUsername->text(), password);
}

void PasswordDialog::validatePasswordError(int error)
{
    switch(error)
    {
    case QNetworkReply::AuthenticationRequiredError:
        ui->labelInfo->setText(tr("<b>Your username or password is incorrect.\nPlease, try again.</b>"));
        break;
    default:
        ui->labelInfo->setText(tr("<b>An unknown error has occured.\nPlease, try again.</b>"));
    }
}

void PasswordDialog::validatePasswordOk(const QString &response)
{
    QScopedPointer<SettingsPassword> settings(new SettingsPassword(this));
    settings->setUsername(ui->editUsername->text());
    if (!ui->editPassword->text().isEmpty())
        settings->setPassword(ui->remember->isChecked() ? QCryptographicHash::hash(ui->editPassword->text().toLocal8Bit(), QCryptographicHash::Md5).toHex() : "");
    settings->setSessionId(response);
    settings->writeSettings();

    if (globalNetwork) globalNetwork->statusLogin();

    accept();
}
