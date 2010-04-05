/****************************************************************************
* SelectPlaylist.cpp: Common playlist selector widget
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* This file may be used under the terms of the
* GNU General Public License version 3.0 as published by the
* Free Software Foundation and appearing in the file LICENSE.GPL
* included in the packaging of this file.
*****************************************************************************/

#include "SelectPlaylist.h"
#include "ui_SelectPlaylist.h"

#include <QtGui/QFileDialog>

#include "core/Settings.h"

SelectPlaylist::SelectPlaylist(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SelectPlaylist)
{
	ui->setupUi(this);

	connect(ui->browsePlaylistButton, SIGNAL(clicked()), this, SLOT(playlistBrowse()));
	connect(ui->resetPlaylistButton, SIGNAL(clicked()), this, SLOT(playlistReset()));
}

SelectPlaylist::~SelectPlaylist()
{
	delete ui;
}

void SelectPlaylist::changeEvent(QEvent *e)
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

QString SelectPlaylist::playlist() const
{
	if(ui->customPlaylistRadio->isChecked())
		return ui->playlistLineEdit->text();
	else if(ui->radioSiol2->isChecked())
		return Settings::PLAYLIST_SIOL_MPEG2;
	else if(ui->radioSiol4->isChecked())
		return Settings::PLAYLIST_SIOL_MPEG4;
	else if(ui->radioT2->isChecked())
		return Settings::PLAYLIST_T2;
	else if(ui->radioTus->isChecked())
		return Settings::PLAYLIST_TUS;
	else if(ui->radioWorldTv->isChecked())
		return Settings::PLAYLIST_WORLDTV;
}

void SelectPlaylist::setPlaylist(const QString &playlist)
{
	if(playlist == Settings::PLAYLIST_SIOL_MPEG2)
		ui->radioSiol2->setChecked(true);
	else if(playlist == Settings::PLAYLIST_SIOL_MPEG4)
		ui->radioSiol4->setChecked(true);
	else if(playlist == Settings::PLAYLIST_T2)
		ui->radioT2->setChecked(true);
	else if(playlist == Settings::PLAYLIST_TUS)
		ui->radioTus->setChecked(true);
	else if(playlist == Settings::PLAYLIST_WORLDTV)
		ui->radioWorldTv->setChecked(true);
	else {
		ui->customPlaylistRadio->setChecked(true);
		ui->playlistLineEdit->setText(playlist);
	}
}

void SelectPlaylist::playlistReset()
{
	ui->playlistLineEdit->setText("");
}

void SelectPlaylist::playlistBrowse()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Open Channel list File"),
												QDir::homePath(),
												tr("Tano TV Channel list Files(*.m3u)"));
	ui->playlistLineEdit->setText(file);
}
