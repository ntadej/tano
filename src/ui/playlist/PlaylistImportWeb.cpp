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

#include <QtGui/QDialogButtonBox>
#include <QtGui/QFileDialog>

#include "core/GetFile.h"

#include "PlaylistImportWeb.h"
#include "ui_PlaylistImportWeb.h"

PlaylistImportWeb::PlaylistImportWeb(QWidget *parent)
	: QDialog(parent),
	ui(new Ui::PlaylistImportWeb),
	_codec(QTextCodec::codecForName("UTF-8"))
{
	ui->setupUi(this);

	_file = new GetFile(this);
	_file->getFile("http://sloiptv.tano.si/playlists/channel-lists-v2.xml");
	connect(_file, SIGNAL(file(QString)), this, SLOT(readList(QString)));

	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(action(QAbstractButton*)));
}

PlaylistImportWeb::~PlaylistImportWeb()
{
	delete ui;
	delete _file;
}

void PlaylistImportWeb::changeEvent(QEvent *e)
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

void PlaylistImportWeb::action(QAbstractButton *button)
{
	switch(ui->buttonBox->standardButton(button)) {
		case QDialogButtonBox::Ok:
			if(!_save) {
				_file->getFile(ui->select->playlist());
				_refresh = ui->radioRefresh->isChecked();
			} else {
				QString file = QFileDialog::getSaveFileName(this, tr("Save channel list"),
															QDir::homePath(),
															tr("Tano TV channel list files(*.m3u)"));
				if(file.isEmpty())
					return;

				_file->getFile(ui->select->playlist(), file);
			}
			connect(_file, SIGNAL(file(QString)), this, SLOT(finish(QString)));
			break;
		case QDialogButtonBox::Cancel:
			_playlist = "";
			_refresh = false;
			close();
			break;
		default:
			break;
	}
}

void PlaylistImportWeb::download()
{
	_save = false;
	exec();
}

void PlaylistImportWeb::save()
{
	ui->typeBox->hide();

	_save = true;
	exec();
}

void PlaylistImportWeb::finish(const QString &playlist)
{
	disconnect(_file, SIGNAL(file(QString)), this, SLOT(finish(QString)));

	_playlist = playlist;
	close();
}

void PlaylistImportWeb::readList(const QString &list)
{
	disconnect(_file, SIGNAL(file(QString)), this, SLOT(readList(QString)));

	ui->select->open(list);
}
