/****************************************************************************
* PlaylistPage.cpp: First run wizard - Playlist selection page
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

#include "PlaylistPage.h"
#include "ui_PlaylistPage.h"

#include <QtCore/QDir>
#include <QtGui/QFileDialog>

#include "core/Settings.h"
#include "FirstRunWizard.h"

PlaylistPage::PlaylistPage(QWidget *parent) :
	QWizardPage(parent),
	ui(new Ui::PlaylistPage),
	_edit(new QLineEdit())
{
	ui->setupUi(this);

	connect(ui->browsePlaylistButton, SIGNAL(clicked()), this, SLOT(playlistBrowse()));
	connect(ui->resetPlaylistButton, SIGNAL(clicked()), this, SLOT(playlistReset()));

	registerField("playlist", _edit);
}

PlaylistPage::~PlaylistPage()
{
	delete ui;
	delete _edit;
}

void PlaylistPage::changeEvent(QEvent *e)
{
	QWizardPage::changeEvent(e);
	switch (e->type()) {
		case QEvent::LanguageChange:
			ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

int PlaylistPage::nextId() const
{
	if(ui->customPlaylistRadio->isChecked())
		_edit->setText(ui->playlistLineEdit->text());
	else if(ui->radioSiol2->isChecked())
		_edit->setText(Settings::PLAYLIST_SIOL_MPEG2);
	else if(ui->radioSiol4->isChecked())
		_edit->setText(Settings::PLAYLIST_SIOL_MPEG4);
	else if(ui->radioT2->isChecked())
		_edit->setText(Settings::PLAYLIST_T2);
	else if(ui->radioTus->isChecked())
		_edit->setText(Settings::PLAYLIST_TUS);

	return FirstRunWizard::Conclusion;
}

void PlaylistPage::playlistReset()
{
	ui->playlistLineEdit->setText("");
}

void PlaylistPage::playlistBrowse()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Open Channel list File"),
												QDir::homePath(),
												tr("Tano TV Channel list Files(*.m3u)"));
	ui->playlistLineEdit->setText(file);
}
