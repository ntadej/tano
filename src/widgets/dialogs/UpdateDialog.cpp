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

#include <QtCore/QTextCodec>
#include <QtGui/QDesktopServices>
#include <QtXml/QXmlInputSource>
#include <QtXml/QXmlSimpleReader>

#include "Config.h"
#include "core/Common.h"
#include "core/network/NetworkRequest.h"
#include "core/plugins/Plugins.h"

#include "UpdateDialog.h"
#include "ui_UpdateDialog.h"

UpdateDialog::UpdateDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::UpdateDialog),
      _codec(QTextCodec::codecForName("UTF-8")),
      _handler(new UpdateHandler()),
      _request(new NetworkRequest(this)),
      _silent(true)
{
    setWindowModality(Qt::WindowModal);

    ui->setupUi(this);

    ui->labelIconDate->setPixmap(QIcon::fromTheme("x-office-calendar").pixmap(22));
    ui->labelIconDev->setPixmap(QIcon::fromTheme("applications-development").pixmap(22));
    ui->labelIconInfo->setPixmap(QIcon::fromTheme("dialog-information").pixmap(22));
    ui->labelIconLatest->setPixmap(QIcon::fromTheme("dialog-ok").pixmap(22));
    ui->labelIconNew->setPixmap(QIcon::fromTheme("dialog-ok").pixmap(22));
    ui->labelIconOld->setPixmap(QIcon::fromTheme("dialog-warning").pixmap(22));
    ui->labelIconStable->setPixmap(QIcon::fromTheme("dialog-ok").pixmap(22));

    ui->buttonDownload->hide();

    connect(ui->buttonClose, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->buttonDownload, SIGNAL(clicked()), this, SLOT(download()));
    connect(ui->buttonChangelog, SIGNAL(clicked()), this, SLOT(changelog()));
    connect(ui->buttonBack, SIGNAL(clicked()), this, SLOT(changelogBack()));

    connect(_request, SIGNAL(result(QByteArray, QNetworkReply *)), this, SLOT(readUpdates(QByteArray, QNetworkReply *)));
}

UpdateDialog::~UpdateDialog()
{
    delete ui;
}

void UpdateDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        if (isHidden())
            ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void UpdateDialog::download()
{
#if defined(Q_OS_WIN)
    QString platform = QString("windows/%1_%2_win%3.exe").arg(Tano::name(), _versionNew, Tano::is64bit() ? "64" : "32");
#elif defined(Q_OS_MAC)
    QString platform = QString("osx/%1_%2.dmg").arg(Tano::name(), _versionNew);
#else
    QString platform = QString("src/%1_%2_src.tar.gz").arg(Tano::name(), _versionNew);
#endif
}

void UpdateDialog::changelog()
{
    ui->main->setCurrentIndex(3);
}

void UpdateDialog::changelogBack()
{
    ui->main->setCurrentIndex(2);
}

void UpdateDialog::check()
{
    if (!globalConfig)
        return;

    _silent = false;
    _currentReply = _request->getRequest(QNetworkRequest(QUrl(globalConfig->updateUrl() + "/update.xml")));
}

void UpdateDialog::checkSilent()
{
    if (!globalConfig)
        return;

    _silent = true;
    _currentReply = _request->getRequest(QNetworkRequest(QUrl(globalConfig->updateUrl() + "/update.xml")));
}

void UpdateDialog::processUpdate(const QStringList &update,
                                 const UpdateInfo &info)
{
    if (update[0] == "latest") {
        setMinimumSize(QSize(450, 150));
        setMaximumSize(QSize(450, 150));

        ui->labelVersionLatest->setText("<b>" + Tano::version() + "</b>");
        ui->main->setCurrentIndex(0);
    } else if (update[0] == "development") {
        setMinimumSize(QSize(450, 200));
        setMaximumSize(QSize(450, 200));

        ui->labelVersionDev->setText("<b>" + update[1] + "</b>");
        ui->labelVersionStable->setText("<b>" + update[2] + "</b>");
        ui->main->setCurrentIndex(1);
    } else if (update[0] == "update") {
        setMinimumSize(QSize(450, 250));
        setMaximumSize(QSize(450, 250));

        ui->labelVersionOld->setText("<b>" + Tano::version() + "</b>");
        ui->labelVersionNew->setText("<b>" + update[1] + "</b>");
        ui->labelDate->setText("<b>" + info.date + "</b>");
        ui->labelInfo->setText(info.description);
        ui->labelChangelog->setText(tr("<h3>Changes in %1</h3>").arg(update[1]));
        ui->main->setCurrentIndex(2);

        _versionNew = update[1];
    }

    if(!_silent || update[0] == "update")
        exec();
}

void UpdateDialog::readUpdates(const QByteArray &data,
                               QNetworkReply *reply)
{
    if (reply != _currentReply)
        return;

    QString string = _codec->toUnicode(data);

    QXmlSimpleReader reader;
    reader.setContentHandler(_handler);
    reader.setErrorHandler(_handler);

    QXmlInputSource xmlInputSource;
    xmlInputSource.setData(string);
    if (!reader.parse(xmlInputSource))
        return;

    QStringList updatesList;
    QList<UpdateInfo> list = _handler->updateInfo();
    UpdateInfo info;

    foreach (const UpdateInfo &item, list) {
        if(Tano::version() == item.version && item.development) {
            updatesList << "development" << item.version;
            continue;
        } else if(Tano::version() == item.version && !item.development) {
            updatesList << "latest";
            break;
        } else if(Tano::version() != item.version && item.development) {
            continue;
        } else if(Tano::version() != item.version && !item.development) {
            if(!updatesList.isEmpty()) {
                updatesList << item.version;
            } else {
                updatesList << "update" << item.version;
                info = item;
            }
        }
    }

    processUpdate(updatesList, info);
}
