/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak <info@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <QtCore/QDebug>
#include <QtGui/QDialogButtonBox>

#include "core/Version.h"

#include "UpdateDialog.h"
#include "ui_UpdateDialog.h"

UpdateDialog::UpdateDialog(QWidget *parent)
	: QDialog(parent),
	ui(new Ui::UpdateDialog),
	_update(new UpdateManager(this)),
	_silent(true)
{
	ui->setupUi(this);

	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(action(QAbstractButton*)));
	connect(_update, SIGNAL(updateInfo(QStringList,UpdateInfo)), this, SLOT(processUpdate(QStringList,UpdateInfo)));
}

UpdateDialog::~UpdateDialog()
{
	delete ui;
}

void UpdateDialog::changeEvent(QEvent *e)
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

void UpdateDialog::action(QAbstractButton *button)
{
	switch(ui->buttonBox->standardButton(button)) {
		case QDialogButtonBox::Ok:
			hide();
			break;
		case QDialogButtonBox::Cancel:
			close();
			break;
		default:
			break;
	}
}

void UpdateDialog::check()
{
	_silent = false;
	_update->getUpdates();
}

void UpdateDialog::checkSilent()
{
	_silent = true;
	_update->getUpdates();
}

void UpdateDialog::processUpdate(const QStringList &update,
								 const UpdateInfo &info)
{
	qDebug() << info.name()
			<< info.version()
			<< info.date()
			<< info.description()
			<< info.development();

	if (update[0] == "latest") {
		ui->labelVersionLatest->setText("<b>" + Version::version() + "</b>");
		ui->main->setCurrentIndex(0);
	} else if (update[0] == "development") {
		ui->labelVersionDev->setText("<b>" + update[1] + "</b>");
		ui->labelVersionStable->setText("<b>" + update[2] + "</b>");
		ui->main->setCurrentIndex(1);
	} else if (update[0] == "update") {
		ui->labelVersionOld->setText("<b>" + Version::version() + "</b>");
		ui->labelVersionNew->setText("<b>" + update[1] + "</b>");
		ui->labelDate->setText("<b>" + info.date() + "</b>");
		ui->labelInfo->setText(info.description());
		ui->labelDownload->setText("<a href=\""+generateUrl(info.version())+"\"><b>"+tr("Download now!")+"</b></a>");
		ui->main->setCurrentIndex(2);

		emit newUpdate();
	}

	if(!_silent)
		exec();
}

QString UpdateDialog::generateUrl(const QString &version)
{
	QString url("http://update.tano.si/player/");
	url.append(version);
	url.append("/");

#if defined(Q_WS_WIN)
	url.append("windows");
#elif defined(Q_WS_MAC)
	url.append("mac");
#else // Q_WS_X11
	url.append("linux");
#endif // Q_WS_*

	url.append("/");

	return url;
}
