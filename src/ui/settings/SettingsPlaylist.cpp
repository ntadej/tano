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

#include <QtGui/QFileDialog>

#include "SettingsPlaylist.h"
#include "ui_SettingsPlaylist.h"

#include "core/Playlists.h"
#include "core/Settings.h"
#include "ui/playlist/PlaylistImportWeb.h"

SettingsPlaylist::SettingsPlaylist(QWidget *parent)
	: QWidget(parent),
	ui(new Ui::SettingsPlaylist)
{
	ui->setupUi(this);

	connect(ui->browsePlaylistButton, SIGNAL(clicked()), this, SLOT(playlistBrowse()));
	connect(ui->downloadButton, SIGNAL(clicked()), this, SLOT(playlistDownload()));
	connect(ui->resetPlaylistButton, SIGNAL(clicked()), this, SLOT(playlistReset()));
}

SettingsPlaylist::~SettingsPlaylist()
{
	delete ui;
}

void SettingsPlaylist::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
		case QEvent::LanguageChange:
			ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

QString SettingsPlaylist::playlist() const
{
	if(ui->customPlaylistRadio->isChecked())
		return ui->playlistLineEdit->text();
	// International
	else if(ui->radioWorldTv->isChecked())
		return TanoPlaylists::International::WorldTv;
	// Russia
	else if(ui->radioDiselKrasnodar->isChecked())
		return TanoPlaylists::Russia::DiselTvKrasnodar;
	else if(ui->radioDiselRostov->isChecked())
		return TanoPlaylists::Russia::DiselTvRostov;
	// Slovenia
	else if(ui->radioAmis->isChecked())
		return TanoPlaylists::Slovenia::Amis;
	else if(ui->radioSiol2->isChecked())
		return TanoPlaylists::Slovenia::Siol2;
	else if(ui->radioSiol4->isChecked())
		return TanoPlaylists::Slovenia::Siol4;
	else if(ui->radioT2->isChecked())
		return TanoPlaylists::Slovenia::T2;
	else if(ui->radioTus->isChecked())
		return TanoPlaylists::Slovenia::Tus;

}

void SettingsPlaylist::setPlaylist(const QString &playlist)
{
	if(playlist == TanoPlaylists::International::WorldTv) {
		ui->radioWorldTv->setChecked(true);
		ui->playlistLineEdit->clear();
	} else if(playlist == TanoPlaylists::Russia::DiselTvKrasnodar) {
		ui->radioDiselKrasnodar->setChecked(true);
		ui->playlistLineEdit->clear();
	}  else if(playlist == TanoPlaylists::Russia::DiselTvRostov) {
		ui->radioDiselRostov->setChecked(true);
		ui->playlistLineEdit->clear();
	}  else if(playlist == TanoPlaylists::Slovenia::Amis) {
		ui->radioAmis->setChecked(true);
		ui->playlistLineEdit->clear();
	} else if(playlist == TanoPlaylists::Slovenia::Siol2) {
		ui->radioSiol2->setChecked(true);
		ui->playlistLineEdit->clear();
	} else if(playlist == TanoPlaylists::Slovenia::Siol4) {
		ui->radioSiol4->setChecked(true);
		ui->playlistLineEdit->clear();
	} else if(playlist == TanoPlaylists::Slovenia::T2) {
		ui->radioT2->setChecked(true);
		ui->playlistLineEdit->clear();
	} else if(playlist == TanoPlaylists::Slovenia::Tus) {
		ui->radioTus->setChecked(true);
		ui->playlistLineEdit->clear();
	} else {
		ui->customPlaylistRadio->setChecked(true);
		ui->playlistLineEdit->setText(playlist);
	}
}

void SettingsPlaylist::playlistReset()
{
	ui->playlistLineEdit->setText("");
}

void SettingsPlaylist::playlistBrowse()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Open channel list"),
												QDir::homePath(),
												tr("Tano TV channel list files(*.m3u)"));
	ui->playlistLineEdit->setText(file);
}

void SettingsPlaylist::playlistDownload()
{
	PlaylistImportWeb web;
	web.save();

	if(web.playlist().isEmpty())
		return;

	ui->customPlaylistRadio->setChecked(true);
	ui->playlistLineEdit->setText(web.playlist());
}
